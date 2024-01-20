#include "netdbg.h"

void netdbg_icmpv6_rdpsol
  (unsigned char* ipv6, unsigned char* icmp, unsigned size)
{
  if (size > 8) {
    unsigned char* options = icmp + 8;
    unsigned optionsize = size - 8;
    netdbg_icmpv6_ndpoptions(options, optionsize);
  }
}
