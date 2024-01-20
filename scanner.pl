#!/usr/bin/perl

my $configfile = shift;
my $configperl = `cat $configfile`;
my $config = eval($configperl);

foreach my $dir (@{$config->{dirs}}) {
  find($dir, 'examine_file');
}

sub examine_file
{
  my ($dir, $file) = @_;
  my $path = "$dir/$file";
  foreach my $policy (@{$config->{policy}}) {
    if ($policy->{regex}) {
      if ($policy->{negate}) {
        next if ($path =~ /$policy->{regex}/);
      } else {
        next if ($path !~ /$policy->{regex}/);
      }
    }
  }
  ##.. absorb the file in the database
}

exit 0;

##---- functions ----##

sub find {
  my $dir = shift || '.';
  my $func = shift;
  my $norecurse = shift;
  if (opendir(DIR, $dir)) {
    my @subdirs;
    my @files;
    while (my $entry = readdir(DIR)) {
      next if ($entry eq '.' || $entry eq '..');
      my $path = "$dir/$entry";
      if (-d $path) {
        push @subdirs, $path;
      } elsif (-f $path) {
        my $ident = $entry;
        push @files, [ $path, $ident ];
      }
    }
    closedir(DIR);
    foreach my $file (sort { $a->[0] cmp $b->[0] } @files) {
      &$func(@{$file});
    }
    if (!$norecurse) {
      foreach my $subdir (sort @subdirs) {
        find($subdir, $func);
      }
    }
  }
}

1;
