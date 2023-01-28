#!/usr/bin/perl

use POSIX ":sys_wait_h";

my $descriptionfile = shift @ARGV || die "Need description file";

my $descriptiontext = `cat $descriptionfile`;
my $description = eval $descriptiontext;

my $result = 1;
my $failed = 0;
my $stopped = 0;

my $time;

$SIG{CHLD} = sub {
  while( ( my $child = waitpid( -1, &WNOHANG ) ) > 0 ) {
    print STDERR "\@$time: PID $child SIGCHLD\n";
    foreach my $nodename (keys(%{$description->{nodes}})) {
      my $node = $description->{nodes}{$nodename};
      foreach my $test (@{$node->{tests}}) {
        if ($test->{pid} eq $child) {
          my $exitcode = $?;
          print STDERR
            "\@$time: PID $test->{pid} $test->{executable} EXIT $exitcode\n";
          if ($test->{critical} && $exitcode ne '0') {
            $failed = 1;
          }
          if ($test->{pivotal}) {
            $stopped = 1;
          }
          delete $test->{pid};
        }
      }
    }
    foreach my $test (@{$description->{tests}}) {
      if ($test->{pid} eq $child) {
        my $exitcode = $?;
        print "\@$time: PID $test->{pid} $test->{executable} EXIT $exitcode\n";
        if ($test->{critical} && $exitcode ne '0') {
          $failed = 1;
        }
        if ($test->{pivotal}) {
          $stopped = 1;
        }
        delete $test->{pid};
      }
    }
  }
};

if ($description->{prepare}) {
  system($description->{prepare});
}
test($description);
if ($description->{finish}) {
  system($description->{finish});
}

print STDERR "Test returns $result\n";
exit $result;

##---- funcs ---------------------------------------------------------------##

sub test {
  my $description = shift;
  my @tests;

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

## Tests

  foreach my $nodename (keys(%{$description->{nodes}})) {
    my $node = $description->{nodes}{$nodename};
    if ($node->{tests}) {
      foreach my $test (@{$node->{tests}}) {
        $test->{namespace} = $nodename;
        push @tests, $test;
      }
    }
  }
  if ($description->{tests}) {
    push @tests, @{$description->{tests}};
  }
  my @start;
  my @stop;
  foreach my $test (@tests) {

    ## Implement aliasing
    if ($test->{e}) { $test->{executable} = $test->{e}; }
    if ($test->{s}) { $test->{start} = $test->{s}; }
    if (!defined($test->{duration})) { $test->{untilend} = 1; }

    push @{$start[ $test->{start} ]}, $test;
    if (defined($test->{duration})) {
      push @{$stop[ $test->{start} + $test->{duration} ]}, $test;
    }
  }
  my $max = (scalar(@start) > scalar(@stop) ? scalar(@start) : scalar(@stop));
  if (defined($description->{duration}) && $description->{duration} > $max) {
    $max = $description->{duration};
  }
  print STDERR "Test takes $max seconds.\n";
  foreach my $test (@tests) {
    if (defined($test->{untilend})) {
      push @{$stop[ ($max - 1) ]}, $test;
    }
  }
  foreach my $key (keys(%{$description->{env}})) {
    $ENV{$key} = $description->{env}{$key};
  }
  my $t = localtime();
  print STDERR "Starting tests at $t\n";
  $stopped = 0;
  for (my $i=0; $i < $max && !$stopped && !$failed; $i++) {
    $time = $i;
    print STDERR "\@$i: tick.\n";
    foreach my $test (@{$start[ $i ]}) {
      my $cmd = $test->{executable};

      if ($test->{namespace}) {
        $cmd = "ip netns exec $test->{namespace} $cmd";
      }
      my $_cmd = "$cmd"; $_cmd =~ s/\n/ /g;
      print STDERR "\@$i: START $_cmd\n";

      foreach my $key (keys(%{$test->{env}})) {
        $ENV{$key} = $test->{env}{$key};
      }
      my $pid = fork();
      die "unable to fork: $!" unless defined($pid);
      if (!$pid) {  # child
        exec($cmd);
        die "unable to exec: $!";
      }
      $test->{pid} = $pid;
      print STDERR "\@$i: PID $pid: $_cmd\n";

    }
    foreach my $test (@{$stop[ $i ]}) {
      my @childpids = get_child_pids($test->{pid});
      my $_cmd = "$test->{executable}"; $_cmd =~ s/\n/ /g;
      print STDERR "\@$i: PID $test->{pid}: STOP $_cmd\n";
      foreach my $childpid (@childpids) {
        print STDERR "\@$time: Issueing kill -INT $childpid\n";
        system("kill -INT $childpid");
      }
      if ($test->{pid}) {
        print STDERR "\@$time: Issueing kill -INT $test->{pid}\n";
        system("kill -INT $test->{pid}");
      } else {
        #print STDERR "WARNING: NO PID\n$i .. ";
      }
    }
    if ($i > 0) {
      foreach my $test (@{$stop[ $i - 1 ]}) {
        my @childpids = get_child_pids($test->{pid});
        my $_cmd = "$test->{executable}"; $_cmd =~ s/\n/ /g;
        print STDERR "\@$i: PID $test->{pid}: WAIT $_cmd\n";
        foreach my $childpid (@childpids) {
          print STDERR "\@$time: Issueing kill -TERM $childpid\n";
          system("kill -TERM $childpid");
        }
        if ($test->{pid}) {
          print STDERR "\@$time: Issueing kill -TERM $test->{pid}\n";
          system("kill -TERM $test->{pid}");
          waitpid($test->{pid}, WNOHANG);
        } else {
          #print STDERR "WARNING: NO PID\n$i .. ";
        }
      }
    }
    #print STDERR "\n";
    sleep(1);
  }
  if ($failed) {
    print STDERR "FAILURE Breaking out of loop because critical process failed\n";
  }
  if ($stopped) {
    print STDERR "FAILURE Breaking out of loop because pivotal process exited\n";
  }
  my $t = localtime();
  print STDERR "Stopping tests at $t\n";
  foreach my $test (@tests) {
    if ($test->{pid}) {
      print STDERR "\@$time: Issueing kill -KILL $test->{pid}\n";
      system('kill', '-KILL', $test->{pid});
      print STDERR "Really killing PID $test->{pid}\n";
      waitpid($test->{pid}, WNOHANG);
    }
  }
  foreach my $test (@tests) {
    if ($test->{pid}) {
      print STDERR "Cleaning up PID $test->{pid}\n";
      waitpid($test->{pid}, WNOHANG);
    }
  }
  if ($description->{success}) {
    $result = system($description->{success});
    if ($result eq '0' && $failed eq '0') {
      print STDERR "SUCCESS $description->{success} => $result\n";
    } else {
      print STDERR "FAILURE $description->{success} => $result\n";
    }
  }
}

sub get_child_pids
{
  my $pid = shift;
  return () if (!$pid);
  my $children = `ps -o pid --ppid $pid`;
  $children =~ s/^ *PID\s*//;
  @children = split(/\s+/, $children);
  return @children;
}

1;
