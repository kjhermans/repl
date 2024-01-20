#!/bin/sh

set -e

diff /tmp/repl/test1.data /tmp/repl/out/test1.data
diff /tmp/repl/test2.data /tmp/repl/out/test2.data
diff /tmp/repl/test3.data /tmp/repl/out/test3.data
diff /tmp/repl/test4.data /tmp/repl/out/test4.data
