#!/usr/bin/perl

use strict;
use IO::Socket;
use Algorithm::Hamming::Perl qw(unhamming);
use Time::HiRes qw(usleep gettimeofday);

my $recvdir = shift || '/tmp/receiver';
my $recoverdir = shift || '/tmp/recover';
my $serverport = shift;
my $udpsize = 1024;
my $chunks = {};
my $lastcounter = 0;

if (! -d $recvdir) {
  mkdir $recvdir;
}

my $sock = IO::Socket::INET->new(
  LocalPort => $serverport,
  Proto     => "udp"
) || die "Couldn't be a udp server on port $serverport : $@\n";

while (1) {
  my $datagram;
  my $n = $sock->recv($datagram, $udpsize * 2);
  if ($n) {
    process($datagram);
  } else {
    usleep(1000);
  }
}

exit 0;

##---- functions -----------------------------------------------------------##

sub process
{
  my $payload = shift;
  my $prefix = unhamming(substr($payload, 0, (4 * 8 * 1.5)));
  my $data = substr($payload, (4 * 8 * 1.5));
  my ($counter, $chunk, $chunksize, $fragno) = unpack('QQQQ', $prefix);
  if ($counter < $lastcounter) {
    return;
  }
  my $file = sprintf("%s/chunk_%.8d_%.8d", $recvdir, $counter, $chunk);
  my $chunkrecord = $chunks->{$chunk};
  if (!defined($chunkrecord)) {
    $chunkrecord = {
      t => gettimeofday(),
      counter => $counter,
      index => $chunk,
      size => $chunksize,
      frags => {},
    };
    $chunks->{$chunk} = $chunkrecord;
    print STDERR "Generating chunk record $counter/$chunk\n";
    system("touch $file");
  } else {
    if ($chunkrecord->{passed}) {
      return;
    }
  }
  $chunkrecord->{frags}{$fragno} = 1;
  if (open FILE, "+< $file") {
    sysseek(FILE, $fragno * $udpsize, 0);
    syswrite(FILE, $data);
    close FILE;
  } else {
    warn "Could not open $file for fragment insertion";
  }
  if (diagnose_chunk($chunkrecord)) {
    print STDERR "Chunk $counter/$chunk is ready and moved to $recoverdir.\n";
    system("mv $file $recoverdir");
    $chunkrecord->{passed} = 1;
  }
}

sub diagnose_chunk
{
  my $record = shift;
  my $expectedfrags = int($record->{size} / $udpsize) + 1;
  if (scalar(keys(%{$record->{frags}})) == $expectedfrags) {
    return 1;
  }
  ## more testing, mostly to do with timeout
  return 0;
}

1;
