#include "netdbg.h"

void netdbg_arp
  (unsigned char* packet, unsigned size)
{
  NETDBG_PRINT( "-------- ARP\n");
  if (packet[4] == 6 && packet[5] == 4) {
    _16BIT oper = GET_16BIT_NWO(packet, 6);
    switch (oper) {
    case 1:
      NETDBG_PRINT( "Oper:    request\n");
      break;
    case 2:
      NETDBG_PRINT( "Oper:    reply\n");
      break;
    default:
      NETDBG_PRINT( "Oper:    %u (Unknown)\n", oper);
      break;
    }
    NETDBG_PRINT( "SHA:     %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
      packet[8], packet[9], packet[10], packet[11], packet[12], packet[13]
    );
    NETDBG_PRINT( "SPA:     %d.%d.%d.%d\n",
      packet[14], packet[15], packet[16], packet[17]
    );
    NETDBG_PRINT( "THA:     %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
      packet[18], packet[19], packet[20], packet[21], packet[22], packet[23]
    );
    NETDBG_PRINT( "TPA:     %d.%d.%d.%d\n",
      packet[24], packet[25], packet[26], packet[27]
    );

    if (oper == 1 &&
        packet[18] == 0 && packet[19] == 0 && packet[20] == 0 &&
        packet[21] == 0 && packet[22] == 0 && packet[23] == 0)
    {
      NETDBG_PRINT(
        "Transl:  Proper request; who-has %d.%d.%d.%d, "
        "tell %d.%d.%d.%d at %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
        packet[24], packet[25], packet[26], packet[27],
        packet[14], packet[15], packet[16], packet[17],
        packet[8], packet[9], packet[10], packet[11], packet[12], packet[13]
      );
    } else if (oper == 1 &&
               memcmp(packet + 8, packet + 18, 6) == 0 &&
               memcmp(packet + 14, packet + 24, 4) == 0)
    {
      NETDBG_PRINT(
        "Transl:  Gratuitous ARP; %d.%d.%d.%d is-at "
        "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
        packet[14], packet[15], packet[16], packet[17],
        packet[8], packet[9], packet[10], packet[11], packet[12], packet[13]
      );
    } else if (oper == 2) {
      NETDBG_PRINT(
        "Transl:  ARP reply; %d.%d.%d.%d is-at "
        "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
        packet[14], packet[15], packet[16], packet[17],
        packet[8], packet[9], packet[10], packet[11], packet[12], packet[13]
      );
    } else {
      NETDBG_PRINT(
        "Transl:  Martian ARP\n"
      );
    }
  } else {
    NETDBG_PRINT(
      "WARNING: Unknown hardware (%d) or protocol (%d) address length\n",
      packet[4], packet[5]
    );
  }
}
