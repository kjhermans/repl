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

my $spooldir = shift;
my $procdir = shift || '/tmp/spoolerproc';
my $counter = 1;
my $udpsize = 1024;
my $chunksize = $udpsize * 256;

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
  print STDERR "Found $file.\n";
  open IN, "< $file" || warn "Could not open file $file.";
  while (1) {
    my $buffer;
    my $n = sysread(IN, $buffer, $chunksize);
    ## Notice that we forward zero sized reads, as a means to signal EOF.
    my $prefix = pack('QQQ', $counter, $chunk, $n);
    my $hammed = hamming($prefix . $buffer);
    my $out =
      sprintf("%s/chunk_%.8d_%.8d_%.8d", $procdir, $counter, $chunk, $n);
    if (!(open OUT, "> $out")) {
      ## recovery procedure, should never happen
      my $glob = sprintf("%s/chunk_%.8d*", $procdir, $counter);
      system("rm -f $glob");
      close IN;
      return 0;
    }
    syswrite(OUT, $hammed);
    close(OUT);
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
