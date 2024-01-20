OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')
MAIN=bridge
CC=$(ARCH)gcc
AR=$(ARCH)ar
INCLUDE=-Inetdbg

CFLAGS=-g -Wall -Wextra $(DEBUG) $(INCLUDE)

all: dbg $(MAIN)

dbg:
	cd netdbg && make

$(MAIN): $(OBJECTS)
	$(CC) -o $(MAIN) $(OBJECTS) netdbg/libnetdbg.a

clean:
	rm -f $(OBJECTS) $(MAIN)
	cd netdbg && make clean
