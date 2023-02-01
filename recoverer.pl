#!/usr/bin/perl

use strict;
use Algorithm::Hamming::Perl qw(unhamming);
use Time::HiRes qw(usleep gettimeofday);

my $indir = shift;
my $outdir = shift;
print STDERR "==== Recoverer\n\nRecovering from $indir to $outdir\n";

my $config = eval(`cat config.pl`);
my $udpsize = $config->{udpsize} || 1024;
my $chunksize = $config->{chunksize} || $udpsize * 256;
print STDERR "Chunksize $chunksize\n";

if (`whoami` eq 'root') {
  die "Don't run this process as root please.";
}

while (1) {
  my $n = process($indir);
  usleep(1000);
}

exit 0;

##---- functions -----------------------------------------------------------##

sub process
{
  my $dir = shift;
  my $n = 0;
  my $admin = {};
  opendir DIR, $dir || die "Could not open dir '$dir'";
  while (my $entry = readdir DIR) {
    next if ($entry eq '.' || $entry eq '..');
    if (-f "$dir/$entry") {
      if (!processfile("$dir/$entry", $admin)) {
        return 0;
      }
      ++$n;
    }
  }
  closedir DIR;
  if ($n) {
    processadmin($admin);
  }
  return $n;
}

sub processfile
{
  my $file = shift;
  my $admin = shift;
  my $n = 0;
  if ($file =~ /^.*\/chunk_([0-9]+)_([0-9]+)$/) {
    print STDERR "Processing $file\n";
    my ($counter, $chunk) = ($1, $2);
    my $hammed = absorb_binary($file);
    my $unhammed = unhamming($hammed);
#use Data::HexDump;
#print STDERR HexDump($unhammed);
    my $chunkdata = substr($unhammed, 4 * 8);
    my ($_counter, $_filesize, $_chunk, $_chunksize) =
      unpack('QQQQ', substr($unhammed, 0, 4 * 8));
    my $chunksize = length($chunkdata);
## do checks
## write decoded chunk
    {
      my $replfile = "$file";
      $replfile =~ s/\/chunk_([0-9]+)_([0-9]+)$/\/decoded_$1_$2/;
      $replfile .= sprintf("_%.8d", $_filesize);
      if (open FILE, "> $replfile") {
        syswrite FILE, $chunkdata;
        close FILE;
        system("rm $file");
      } else {
        warn "Could not open decoded file for writing at $replfile.\n";
      }
    }
    $admin->{$counter}{$chunk} = {
      filesize => $_filesize,
      chunksize => $chunksize,
    };
    ++$n;
  } elsif ($file =~ /^.*\/decoded_([0-9]+)_([0-9]+)_([0-9]+)$/) {
    my @s = stat $file;
    my $chunksize = $s[ 7 ];
    my ($counter, $chunk, $filesize) = ($1, $2, int($3));
    $admin->{$counter}{$chunk} = {
      filesize => $filesize,
      chunksize => $chunksize,
    };
    ++$n;
  }
}

sub processadmin
{
  my $admin = shift;
  foreach my $counter (keys(%{$admin})) {
    processcounter($counter, $admin->{$counter});
  }
}

sub processcounter
{
  my $counter = shift;
  my $record = shift;

  my $lastchunkno = undef;
  my $size = 0;
  my $_size;
  foreach my $chunkno (sort(map(int, keys(%{$record})))) {
    my $chunkdata = $record->{sprintf("%.8d", $chunkno)};
    $_size = $chunkdata->{filesize};
    $size += $chunkdata->{chunksize};
    if (!defined($lastchunkno)) {
      if ($chunkno != 0) {
        return; ## not complete
      }
    } else {
      if ($chunkno != $lastchunkno + 1) {
        return; ## not complete
      }
    }
    $lastchunkno = $chunkno;
  }
  if ($size >= $_size) {
    spinout($counter, $record, $_size, ($size - $_size), $lastchunkno);
  }
}

sub spinout
{
  my ($counter,
      $record,
      $truesize,
      $filenamesize,
      $lastchunkno) = @_;

print STDERR "SPINOUT $truesize $lastchunkno\n";
  my $outfile = sprintf("%s/final_$counter", $indir, $counter);
  my $filename;
  if (!open(OUT, "> $outfile")) {
    warn "Could not open output file $outfile.";
    return;
  }
  for (my $i=0; $i < $lastchunkno+1; $i++) {
print STDERR "CHUNK $i\n";
    my $chunk = sprintf("%.8", $i);
    my $file =
      sprintf("%s/decoded_%.8d_%.8d_%.8d", $indir, $counter, $i, $truesize);
    if (open(FILE, "< $file")) {
print STDERR "OGOGOGO TO HERE.\n";
      my @s = stat FILE;
      my $buffer;
      my $n = sysread(FILE, $buffer, $s[ 7 ]);
      ## check $n
      close FILE;
      if ($i == $lastchunkno) {
        $filename = substr($buffer, length($buffer) - $filenamesize);
        $filename =~ s/^.//; ## check whether this was the null byte
        $buffer = substr($buffer, 0, length($buffer) - $filenamesize);
      }
      syswrite(OUT, $buffer);
    } else {
      warn "Could not open $file for final spinout";
    }
  }
print STDERR "GOT TO HERE.\n";
  close(OUT);
  system("mv $outfile $outdir/$filename");
print STDERR "FILENAME = $filename\n";
system("ls -l $outdir/");
system("hexdump -C $outdir/$filename");
  my $glob = sprintf("%s/decoded_%.8d_*", $indir, $counter);
  system("rm -f $glob");
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
