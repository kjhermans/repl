#!/bin/sh

perl network.pl \
  test.network \
  off

./catter.sh /tmp/repl/tcpdump.0.log
./catter.sh /tmp/repl/spooler.log
./catter.sh /tmp/repl/processor.log
./catter.sh /tmp/repl/receiver.log
./catter.sh /tmp/repl/recoverer.log
./catter.sh /tmp/repl/diode.log
./catter.sh /tmp/repl/arp.log
ls -l /tmp/repl/recv
ls -l /tmp/repl/reco

rm -rf /tmp/repl/
