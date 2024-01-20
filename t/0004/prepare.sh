#!/bin/sh

rm -rf /tmp/repl
mkdir /tmp/repl
mkdir /tmp/repl/spool
mkdir /tmp/repl/proc

mkdir /tmp/repl/recv
mkdir /tmp/repl/reco
mkdir /tmp/repl/out

head -c 64 /dev/random | xxd > /tmp/repl/test1.data

perl network.pl \
  test.network \
  off

perl network.pl \
  test.network \
  on