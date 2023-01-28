#!/bin/sh

rm -rf /tmp/repl
mkdir /tmp/repl
mkdir /tmp/repl/spool
mkdir /tmp/repl/proc
mkdir /tmp/repl/recv
mkdir /tmp/repl/reco

perl network.pl \
  test.network \
  off

perl network.pl \
  test.network \
  on
