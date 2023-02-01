#!/bin/sh

rm -rf /tmp/repl
mkdir /tmp/repl
mkdir /tmp/repl/spool
mkdir /tmp/repl/proc

mkdir /tmp/repl/recv
mkdir /tmp/repl/reco
mkdir /tmp/repl/out

#head -c 512 /dev/random | xxd > /tmp/repl/foo ## Generate test file
i=0
max=256
while [ $i -lt $max ]
do
  echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" >> /tmp/repl/foo
  i=`expr $i + 1`
done

perl network.pl \
  test.network \
  off

perl network.pl \
  test.network \
  on
