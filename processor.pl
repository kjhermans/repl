#!/usr/bin/perl

##
## This script scans the directory with Hammed chunks ready for sending.
## Sending will take place in fragments (UDP packets) to another host.
##

use strict;
use IO::Socket;
use Algorithm::Hamming::Perl qw(hamming);
use Time::HiRes qw(usleep);

my $procdir = shift;
my $host = shift;
my $port = shift;
my $udpsize = 1024;

my $sock = IO::Socket::INET->new(
    Proto    => 'udp',
    PeerPort => $port,
    PeerAddr => $host,
) or die "Could not create socket: $!\n";

if (! -d $procdir) {
  mkdir $procdir || die "Could not find or create $procdir";
}

while (1) {
  my $n = process($procdir);
  if ($n == 0) {
    usleep(1000);
  }
}

exit 0;

##---- functions -----------------------------------------------------------##

sub process
{
  my $dir = shift;
  my $n = 0;
  opendir DIR, $dir || die "Could not open dir '$dir'";
  while (my $entry = readdir DIR) {
    next if ($entry eq '.' || $entry eq '..');
    if (-f "$dir/$entry") {
      if (!processfile("$dir/$entry")) {
        return 0;
      }
      ++$n;
    }
  }
  closedir DIR;
  return $n;
}

sub processfile
{
  my $file = shift;
  print STDERR "Processing $file\n";
  my ($counter, $chunk, $chunksize);
  my $n = 0;
  if ($file =~ /^.*\/chunk_([0-9]+)_([0-9]+)_([0-9]+)$/) {
    ($counter, $chunk, $chunksize) = ($1, $2, $3);
  } else {
    warn "Found file $file, which is not a chunk.";
    return 0;
  }
  my $contents = absorb_binary($file);
  while ($contents =~ s/^(.{$udpsize})//) {
    my $frag = $1;
    my $prefix = pack('QQQQ', $counter, $chunk, $chunksize, $n);
    $sock->send(hamming($prefix) . $frag);
    ++$n;
  }
  my $frag = $contents;
  my $prefix = pack('QQQQ', $counter, $chunk, $chunksize, $n);
  $sock->send(hamming($prefix) . $frag);
  system("rm -f $file");
}

sub absorb_binary
{
  my $path = shift; die "$path not found" if (! -f $path);
  my $result = '';
  die "Error $@ opening $path" if (!open(FILE, '<', $path));
  binmode FILE;
  my $buf;
  while (1) {
    my $n = sysread(FILE, $buf, 1024);
    if (!$n) {
      close FILE;
      return $result;
    }
    $result .= $buf;
  }
}

1;
