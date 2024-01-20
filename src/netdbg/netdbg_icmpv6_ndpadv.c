#include "netdbg.h"

void netdbg_icmpv6_ndpadv
  (unsigned char* ipv6, unsigned char* icmp, unsigned size)
{
  unsigned R = ((icmp[4] >> 7) & 1);
  unsigned S = ((icmp[4] >> 6) & 1);
  unsigned O = ((icmp[4] >> 5) & 1);
  NETDBG_PRINT( "-------- ICMPv6 NDP Neighbor Advertisement\n");
  NETDBG_PRINT( "Bit R:   %u\n", R);
  NETDBG_PRINT( "Bit S:   %u\n", S);
  NETDBG_PRINT( "Bit O:   %u\n", O);
  NETDBG_PRINT( "Address: %x:%x:%x:%x:%x:%x:%x:%x\n",
    GET_16BIT_NWO(icmp, 8),
    GET_16BIT_NWO(icmp, 10),
    GET_16BIT_NWO(icmp, 12),
    GET_16BIT_NWO(icmp, 14),
    GET_16BIT_NWO(icmp, 16),
    GET_16BIT_NWO(icmp, 18),
    GET_16BIT_NWO(icmp, 20),
    GET_16BIT_NWO(icmp, 22)
  );
  if (size > 24) {
    unsigned char* options = icmp + 24;
    unsigned optionsize = size - 24;
    netdbg_icmpv6_ndpoptions(options, optionsize);
  }
}
