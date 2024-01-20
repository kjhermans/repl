#!/bin/sh

set -e

diff /tmp/repl/test1.data /tmp/repl/out/test1.data
diff /tmp/repl/test2.data /tmp/repl/out/test2.data
