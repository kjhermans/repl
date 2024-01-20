#include "netdbg.h"

void netdbg_icmpv6_rdpadv
  (unsigned char* ipv6, unsigned char* icmp, unsigned size)
{
  _16BIT lifetime = GET_16BIT_NWO(icmp, 6);
  _32BIT reachabletime = GET_32BIT_NWO(icmp, 8);
  _32BIT retranstimer = GET_32BIT_NWO(icmp, 12);
  NETDBG_PRINT( "-------- ICMPv6 NDP Router Advertisement\n");
  NETDBG_PRINT( "CurHopL: %u\n", icmp[4]);
  NETDBG_PRINT( "Bit M:   %u\n", ((icmp[5] >> 7) & 1));
  NETDBG_PRINT( "Bit O:   %u\n", ((icmp[5] >> 6) & 1));
  NETDBG_PRINT( "Lifetim: %u\n", lifetime);
  NETDBG_PRINT( "ReachT:  %u\n", reachabletime);
  NETDBG_PRINT( "RetransT %u\n", retranstimer);
  if (size > 16) {
    unsigned char* options = icmp + 16;
    unsigned optionsize = size - 16;
    netdbg_icmpv6_ndpoptions(options, optionsize);
  }
}
