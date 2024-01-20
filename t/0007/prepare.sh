#!/bin/sh

rm -rf /tmp/repl
mkdir /tmp/repl
mkdir /tmp/repl/spool
mkdir /tmp/repl/proc

mkdir /tmp/repl/recv
mkdir /tmp/repl/reco
mkdir /tmp/repl/out

head -c 83 /dev/random | xxd > /tmp/repl/test1.data
head -c 513 /dev/random | xxd > /tmp/repl/test2.data
head -c 1029 /dev/random | xxd > /tmp/repl/test3.data
head -c 4097 /dev/random | xxd > /tmp/repl/test4.data

perl network.pl \
  test.network \
  off

perl network.pl \
  test.network \
  on
