#include "netdbg.h"

void netdbg_icmpv6_ndpsol
  (unsigned char* ipv6, unsigned char* icmp, unsigned size)
{
  NETDBG_PRINT( "-------- ICMPv6 NDP Neighbor Solicitation\n");
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
