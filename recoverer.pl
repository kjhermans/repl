#!/usr/bin/perl

use strict;
use Algorithm::Hamming::Perl qw(unhamming);
use Time::HiRes qw(usleep gettimeofday);

my $indir = shift;
my $outdir = shift;

while (1) {
  my $n = process($indir);
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
}

1;
