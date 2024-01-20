#!/bin/sh

perl network.pl \
  test.network \
  off

echosep ()
{
  echo
  echo
  echo "++++"
  echo
  echo
}

echosep
cat /tmp/repl/tcpdump.0.log
echosep
cat /tmp/repl/spooler.log
echosep
cat /tmp/repl/processor.log
echosep
cat /tmp/repl/receiver.log
echosep
cat /tmp/repl/recoverer.log
echosep
cat /tmp/repl/diode.log
echosep
cat /tmp/repl/arp.log
echosep

rm -rf /tmp/repl/
