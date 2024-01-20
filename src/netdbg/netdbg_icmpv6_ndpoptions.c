#include "netdbg.h"

void netdbg_icmpv6_ndpoptions
  (unsigned char* options, unsigned size)
{
  while (size >= 8) {
    unsigned optiontype = options[0];
    unsigned optionlength = options[1];
    NETDBG_PRINT( "-------- NDP Option\n");
    NETDBG_PRINT( "Type:    %u\n", optiontype);
    NETDBG_PRINT( "Length:  %u octets\n", optionlength);
    if (optionlength == 0) {
      NETDBG_PRINT( "WARNING: Option length zero; no further processing.\n");
      return;
    } 
    if (optionlength * 8 > size) {
      NETDBG_PRINT( "ERROR:   Option length exceeds packet.\n");
      return;
    }
    switch (optiontype) {
    case 1:
      NETDBG_PRINT( "-------- NDP Option: Source link-layer address\n");
      if (optionlength != 1) {
        NETDBG_PRINT( "WARNING: Option length not 1.\n");
      }
      NETDBG_PRINT( "Address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
        options[2], options[3], options[4],
        options[5], options[6], options[7]
      );
      break;
    case 2:
      NETDBG_PRINT( "-------- NDP Option: Target link-layer address\n");
      if (optionlength != 1) {
        NETDBG_PRINT( "WARNING: Option length not 1.\n");
      }
      NETDBG_PRINT( "Address: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
        options[2], options[3], options[4],
        options[5], options[6], options[7]
      );
      break;
    case 3:
      NETDBG_PRINT( "-------- NDP Option: Router prefix\n");
      if (optionlength != 4) {
        NETDBG_PRINT( "WARNING: Option length not 4.\n");
      }
      {
        NETDBG_PRINT( "Preflen: %u\n", options[2]);
        NETDBG_PRINT( "Bit L:   %u\n", ((options[3] >> 7) & 1));
        NETDBG_PRINT( "Bit A:   %u\n", ((options[3] >> 6) & 1));
        NETDBG_PRINT( "Address: %x:%x:%x:%x:%x:%x:%x:%x\n",
          GET_16BIT_NWO(options, 16),
          GET_16BIT_NWO(options, 18),
          GET_16BIT_NWO(options, 20),
          GET_16BIT_NWO(options, 22),
          GET_16BIT_NWO(options, 24),
          GET_16BIT_NWO(options, 26),
          GET_16BIT_NWO(options, 28),
          GET_16BIT_NWO(options, 30)
        );
      }
      break;
    case 4:
;
    }
    options += (optionlength * 8);
    size -= (optionlength * 8);
  } 
}
