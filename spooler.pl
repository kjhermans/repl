#!/usr/bin/perl

##
## This script scans a spool directory for files, and then rewrites them
## as Hamming-code applied chunks. These chunks are indexed by file number
## and chunk index, which are included in the error correction, so they
## should recover.
##

use strict;
use Algorithm::Hamming::Perl qw(hamming);
use Time::HiRes qw(usleep);

my $counter = 1; ## Chunk counter

my $spooldir = shift;
my $procdir = shift || '/tmp/spoolerproc';
print STDERR "==== Spooler.\n\nSpooling from $spooldir to $procdir...\n";

my $config = eval(`cat config.pl`);
my $udpsize = $config->{udpsize} || 1024;
my $chunksize = $config->{chunksize} || $udpsize * 256;
print STDERR "UDP payload size $udpsize, chunksize $chunksize\n";

if (`whoami` eq 'root') {
  die "Don't run this process as root please.";
}

if (! -d $procdir) {
  mkdir $procdir || die "Could not find or create $procdir";
}

while (1) {
  my $n = spooldir($spooldir);
  if ($n == 0) {
    usleep(1000);
  }
}

print STDERR "Spooler exit.\n";
exit 0;

##---- functions -----------------------------------------------------------##

sub spooldir
{
  my $dir = shift;
  my $n = 0;
  opendir DIR, $dir || die "Could not open dir '$dir'";
  while (my $entry = readdir DIR) {
    next if ($entry eq '.' || $entry eq '..');
    if (-f "$dir/$entry") {
      if (!spoolfile("$dir/$entry")) {
        return 0;
      }
      ++$n;
      ++$counter;
    }
  }
  closedir DIR;
  return $n;
}

sub spoolfile
{
  my $chunk = 0;
  my $file = shift;
  my $basename = "$file";
  $basename =~ s/^.*\/([^\/]+)$/$1/;
  print STDERR "Found $file; transport as $basename.\n";
  if (!open IN, "< $file") {
    warn "Could not open file $file.";
    return 0;
  }
  my @s = stat IN;
  my $size = $s[ 7 ];
  while (1) {
    my $buffer;
    my $n = sysread(IN, $buffer, $chunksize);
    if ($n == 0) {
      last;
    }
    if ($n < $chunksize) { ## last chunk, postfile basename
      $buffer .= "\0" . $basename;
    }
    my $prefix = pack('QQQQ', $counter, $size, $chunk, $n);
## TODO consider: last chunk chunksize may be LONGER than default chunksize
    my $hammed = hamming(
      $prefix .
      $buffer
    );
    my $out =
      sprintf("%s/chunk_%.8d_%.8d_%.8d", $procdir, $counter, $chunk, $n);
    if (!(open OUT, "> $out")) {
      warn "Recovery: could not open output file $out.";
      my $glob = sprintf("%s/chunk_%.8d*", $procdir, $counter);
      system("rm -f $glob");
      close IN;
      return 0;
    }
    syswrite(OUT, $hammed);
    close(OUT);
    print STDERR "Written chunk $out\n";
    $chunk++;
    if ($n < $chunksize) {
      last;
    }
  }
  system("rm -f $file");
  close IN;
  return 1;
}

1;
