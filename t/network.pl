#!/usr/bin/perl

use Data::Dumper;

my $descriptionfile = shift @ARGV;
my $onoff = shift @ARGV;

my $descriptiontext = `cat $descriptionfile`;
my $description = eval $descriptiontext;

preprocess($description);

if ($onoff eq 'on') {
  turnon($description);
  list($description);
} elsif ($onoff eq 'list') {
  list($description);
} else {
  turnoff($description);
}

exit 0;

##---- funcs ---------------------------------------------------------------##

sub do_or_die {
  my $cmd = shift;
  my $msg = shift;
  my $str = shift;
  my $n;
  print join(' ', @{$cmd}) . ' ';
  if ($str) {
    $cmd = join(' ', @{$cmd});
    $n = system($cmd);
  } else {
    $n = system(@{$cmd});
  }
  if ($n) {
    print "## Failed.\n";
    print STDERR "$msg\n";
    turnoff($description);
    exit -1;
  }
  print "## Ok.\n";
}

sub turnon {
  my $description = shift;

## Add the network namespaces

  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    my @cmd = ( qw(ip netns add), $nodename);
    do_or_die(\@cmd, "Couldn't add network namespace $nodename");
  }

# Create the virtual interfaces

  my %connections;
  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    foreach my $interfacename (keys(%{$node->{interfaces}})) {
      my $interface = $node->{interfaces}{$interfacename};
      my $ident = "$nodename$interfacename";
      my $connection = $interface->{connection};
      if (!$connection) {
        if ($interface->{disconnected}) {
          print STDERR "Node $nodename iface $interfacename is disconnected.\n";
          my @cmd = ( qw(ip link add), "$nodename$interfacename", qw(type dummy));
          do_or_die(\@cmd, "Couldn't add dummy interface $nodename$interfacename");
          next;
        }
        print STDERR "Node $nodename iface $interfacename has no connection.\n";
        exit -1;
      }
      if (!$connection->{node} || !$description->{nodes}{$connection->{node}}) {
        print STDERR "Node $nodename iface $interfacename connect node ref.\n";
        exit -1;
      }
      if (!$connection->{interface} ||
          !$description->{nodes}{$connection->{node}}{interfaces}{$connection->{interface}})
      {
        print STDERR "Node $nodename iface $interfacename connect iface ref.\n";
        exit -1;
      }
      $connections{$ident} = "$connection->{node}$connection->{interface}";
    }
  }
  foreach my $connection (keys(%connections)) {
    my $peer = $connections{$connection};
    if ($connections{$peer} ne $connection) {
      print STDERR "Connections aren't reciprocally correctly defined.\n";
      exit -1;
    }
  }
  my %connections2;
  foreach my $connection (keys(%connections)) {
    my $peer = $connections{$connection};
    my ($i1, $i2) = sort ($peer, $connection);
    $connections2{$i1} = $i2;
  }
  foreach my $connection (keys(%connections2)) {
    my $peer = $connections2{$connection};
    my @cmd = ( qw(ip link add), $connection, qw(type veth peer name), $peer);
    do_or_die(\@cmd, "Couldn't add virtual interfaces $connection and $peer");
  }

# Coupling the virtual interfaces to the network namespaces

  my %connections;
  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    foreach my $interfacename (keys(%{$node->{interfaces}})) {
      my $interface = $node->{interfaces}{$interfacename};
      my $ident = "$nodename$interfacename";
      my @cmd = ( qw(ip link set), $ident, 'netns', $nodename);
      do_or_die(\@cmd, "Couldn't bind interface $ident to ns $nodename");
      my @cmd = ( qw(ip netns exec), $nodename, qw(ethtool --offload), $ident, qw(rx off tx off));
      do_or_die(\@cmd, "Couldn't manipulate namespace $nodename");
      my @cmd = ( qw(ip netns exec), $nodename, qw(ethtool -K), $ident, qw(gso off));
      do_or_die(\@cmd, "Couldn't manipulate namespace $nodename");
    }
  }

# IP

  my %connections;
  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    foreach my $interfacename (keys(%{$node->{interfaces}})) {
      my $interface = $node->{interfaces}{$interfacename};
      my $ident = "$nodename$interfacename";
      my @cmd;

      if (($interface->{ip} || $interface->{ipv6})
          && $node->{type} eq 'switch')
      {
        ## A switch may not have interfaces with IP configuration
        print STDERR "Switch $nodename has IP config for $interfacename.\n";
        exit -1;
      }
      if (!$interface->{ip} && !$interface->{ipv6}
          && $node->{type} eq 'router')
      {
        ## A router must have all interfaces with IP configuration
        print STDERR "Router $nodename needs IP config for $interfacename.\n";
        exit -1;
      }

      @cmd = ( qw(ip netns exec), $nodename, 'ifconfig', $ident, 'up');
      do_or_die(\@cmd, "Couldn't bring interface $ident up");
      if ($interface->{vlan}) {
        @cmd = ( qw(ip netns exec), $nodename,
                 qw(ip link add link ), $ident,
                 'name', "$ident.$interface->{vlan}",
                 qw(type vlan id), $interface->{vlan} );
        do_or_die(\@cmd, "Couldn't create VLAN interface.");
        $ident .= '.' . $interface->{vlan};
        @cmd = ( qw(ip netns exec), $nodename, 'ifconfig', $ident, 'up' );
        do_or_die(\@cmd, "Couldn't bring VLAN interface up.");
      }
      if ($interface->{ip}) {
        @cmd = (
          qw(ip netns exec),
          $nodename,
          'ifconfig',
          $ident,
          $interface->{ip}
        );
        do_or_die(\@cmd, "Couldn't assign IPv4 $interface->{ip} to $ident");
      }
      if ($interface->{ipv6}) {
        @cmd = (
          qw(ip netns exec),
          $nodename,
          'ifconfig',
          $ident,
          'inet6',
          'add',
          $interface->{ipv6}
        );
        do_or_die(\@cmd, "Couldn't assign IPv6 $interface->{ipv6} to $ident");
      }
      if ($interface->{ip} && $interface->{route}) {
        my @routes;
        if (ref $interface->{route} eq 'ARRAY') {
          @routes = @{$interface->{route}};
        } else {
          @routes = ( $interface->{route} );
        }
        foreach my $route (@routes) {
          my $ipre = '[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+';
          if ($route =~ /^\s*($ipre\/[0-9]+)\s+($ipre|\w+)\s*$/) {
            my ($segment, $routerip) = ($1, $2);
            my @cmd = (
              qw(ip netns exec), $nodename,
              qw(route add -net), $segment, 'gw', $routerip
            );
            do_or_die(\@cmd, "Couldn't create route $nodename -> $segment");
          }
        }
      }
      if ($interface->{ipv6} && $interface->{routev6}) {
        if ($interface->{routev6} =~ /^\s*(.+)\s+(.+)\s*$/) {
          my ($segment, $gateway) = ($1, $2);
          my @cmd = (
            qw(ip netns exec), $nodename,
            qw(route -A inet6 add), $segment, 'gw', $gateway
          );
          do_or_die(\@cmd, "Couldn't create IPv6 route $nodename -> $segment");
        } else {
          print STDERR "Need two elements in IPv6 route at $nodename $ident\n";
          exit -1;
        }
      }
    }

# IP Routing

    if ($node->{type} eq 'router') {
      my @cmd = (
        qw(ip netns exec), $nodename,
        qw(/bin/sh -c), "'echo 1 > /proc/sys/net/ipv4/ip_forward'"
      );
      do_or_die(\@cmd, "Couldn't set ip_forward for $nodename", 1);

      my @cmd = (
        qw(ip netns exec), $nodename,
        qw(/bin/sh -c), "'echo 1 > /proc/sys/net/ipv6/conf/all/forwarding'"
      );
      do_or_die(\@cmd, "Couldn't set IPv6 forwarding for $nodename", 1);

# Layer 2 switch using a linux bridge

    } elsif ($node->{type} eq 'switch') {
      my $switchname = "switch$nodename";
      my @cmd = (
        qw(ip netns exec), $nodename,
        qw(ip link add name), $switchname, qw(type bridge)
      );
      do_or_die(\@cmd, "Could not create bridge interface $switchname");
      @cmd = (
        qw(ip netns exec), $nodename,
        qw(ip link set dev), $switchname, 'up'
      );
      do_or_die(\@cmd, "Could not bring bridge interface $switchname up");
      foreach my $interfacename (keys(%{$node->{interfaces}})) {
        my $interface = $node->{interfaces}{$interfacename};
        my $ident = "$nodename$interfacename";
        my @cmd = (
          qw(ip netns exec), $nodename,
          qw(ip link set dev), $ident, qw(promisc on)
        );
        do_or_die(\@cmd, "Interface $interfacename promiscuous mode failure");
        @cmd = (
          qw(ip netns exec), $nodename,
          qw(ip link set dev), $ident, 'up'
        );
        do_or_die(\@cmd, "Couldn't bring $interfacename up");
        @cmd = (
          qw(ip netns exec), $nodename,
          qw(ip link set dev), $ident, 'master', $switchname
        );
        do_or_die(\@cmd, "Couldn't bridge $interfacename to $switchname");
      }
    }
  }
}

sub turnoff {
  my $description = shift;
  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    foreach my $interfacename (keys(%{$node->{interfaces}})) {
      my $interface = $node->{interfaces}{$interfacename};
      my $ident = "$nodename$interfacename";
      system('ip', 'netns', 'exec', $nodename, 'ifconfig', $ident, 'down');
      system('ip', 'netns', 'exec', $nodename, 'ip', 'link', 'delete', $ident, 'type', 'veth');
      system('ifconfig', $ident, 'down');
      system('ip', 'link', 'delete', $ident, 'type', 'veth');
    }
    system('ip', 'netns', 'del', $nodename);
    if ($node->{type} eq 'switch') {
      my $switchname = "switch$nodename";
      my @cmd = (
        qw(ip netns exec), $nodename,
        qw(ip link delete name), $switchname, qw(type bridge)
      );
      system(@cmd);
    }
  }
}

sub preprocess {
  my $description = shift;

## Preprocess (includes)

  foreach my $nodename (keys(%{$description->{nodes}})) {
    if ($nodename =~ /^include_/) {
      my $node = $description->{nodes}{$nodename};
      my $includecode = `cat $node->{include}`;
      my $substituted = eval("\$includecode =~ s/$node->{substitute}/g;");
      my $includehash = eval($includecode);
      print STDERR
        "Including $nodename with file $node->{include} and subsituting\n".
#        Dumper($includehash);
'';
      delete $description->{nodes}{$nodename};
      foreach my $key (keys(%{$includehash})) {
        print STDERR "Substituting $key\n";
        $description->{nodes}{$key} = $includehash->{$key};
      }
    }
  }
#print STDERR Dumper($description->{nodes});

  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    foreach my $interfacename (keys(%{$node->{interfaces}})) {
      my $interface = $node->{interfaces}{$interfacename};
      my $connection;
      if (defined($connection = $interface->{connection})) {
        my $peernode = $description->{nodes}{$connection->{node}};
        my $peeriface = $peernode->{interfaces}{$connection->{interface}};
        if (!defined($peernode) || !defined($peeriface)) {
          die "Unresolvable connection defined at $nodename/$interfacename";
        }
        if (!defined($peeriface->{connection})) {
          print STDERR
            "## Notify: Fixing $nodename/$interfacename connection " .
            "to $connection->{node}/$connection->{interface}\n";
          $peeriface->{connection} = {
            node => $nodename,
            interface => $interfacename
          };
        } else {
          if ($peeriface->{connection}{node} ne $nodename
              || $peeriface->{connection}{interface} ne $interfacename)
          {
            print STDERR
              "## Connection between $nodename/$interfacename and ".
              "$connection->{node}/$connection->{interface} is improper.\n" .
              Dumper($description);
            exit -1;
          }
        }
      }
    }
  }
}

sub list {
  my $description = shift;
  print STDERR "\n\n\nNamespaces:\n";
  print STDERR `ip netns list`;
  foreach my $nodename (keys(%{$description->{nodes}})) {
    print STDERR "\n\n\nNamespace $nodename ifconfig and routing table.\n";
#    print STDERR `ip netns exec $nodename ifconfig`;
    print STDERR `ip netns exec $nodename ip -d addr show`;
    print STDERR `ip netns exec $nodename ip route show`;
#    print STDERR `ip netns exec $nodename ip neigh show`;
#    print STDERR `ip netns exec $nodename arp -a`;
#    print STDERR `ip netns exec $nodename route -n`;
    print STDERR `ip netns exec $nodename ip -6 route show`;
    print STDERR "ip_forward = " .
                 `ip netns exec $nodename cat /proc/sys/net/ipv4/ip_forward`;
  }
}

1;
