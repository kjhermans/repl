TARGET=libnetdbg.a

CFLAGS=-g -O2 -Wall

NETDBG_OBJECTS=\
  netdbg_arp.o \
  netdbg_checksum.o \
  netdbg_ethernet.o \
  netdbg_icmp.o \
  netdbg_icmpv6.o \
  netdbg_icmpv6_destunreach.o \
  netdbg_icmpv6_packettoobig.o \
  netdbg_icmpv6_timeexceeded.o \
  netdbg_icmpv6_paramproblem.o \
  netdbg_icmpv6_echorequest.o \
  netdbg_icmpv6_echoreply.o \
  netdbg_icmpv6_mldquery.o \
  netdbg_icmpv6_mldreport.o \
  netdbg_icmpv6_mldleave.o \
  netdbg_icmpv6_rdpsol.o \
  netdbg_icmpv6_rdpadv.o \
  netdbg_icmpv6_ndpsol.o \
  netdbg_icmpv6_ndpadv.o \
  netdbg_icmpv6_ndpoptions.o \
  netdbg_icmpv6_mldreportv2.o \
  netdbg_igmp.o \
  netdbg_ipv4.o \
  netdbg_ipv4_dhcp.o \
  netdbg_ipv4_header.o \
  netdbg_ipv4tcpstate_init.o \
  netdbg_ipv6.o \
  netdbg_ple.o \
  netdbg_print.o \
  netdbg_tcp.o \
  netdbg_udp.o \

all: $(TARGET)

$(TARGET): $(NETDBG_OBJECTS)
	@/bin/echo -n "  [AR] $(TARGET) .. "; \
	if ar -rcs $(TARGET) $(NETDBG_OBJECTS); then \
	  echo "Ok"; \
	else \
	  echo "Failed: ar -rcs $(TARGET) $(NETDBG_OBJECTS)"; \
	  exit -1; \
	fi

clean:
	rm -f $(TARGET) $(NETDBG_OBJECTS)
	find . | grep dSYM | xargs rm -rf
	rm -f test_ethernet test_ipv4

test: all
	$(CC) $(CFLAGS) -o test_ethernet test_ethernet.c libnetdbg.a && \
	  ./test_ethernet
	$(CC) $(CFLAGS) -o test_ipv4 test_ipv4.c libnetdbg.a && \
	  ./test_ipv4

archive: clean
	cd .. && test -d netdbg && tar czvf ~/netdbg.tar.gz netdbg/

%.o: %.c
	@if $(CC) $(CFLAGS) -c -o $@ $<; then \
	  echo "[CC] $< .. Ok"; \
	else \
	  echo "$(CC) $(CFLAGS) -c -o $@ $<"; \
	  exit -1; \
	fi
