#include "netdbg.h"

void netdbg_icmp
  (unsigned char* packet, unsigned size)
{
  int type = packet[0];
  int code = packet[1];
  _16BIT id = GET_16BIT_NWO(packet, 4);
  _16BIT seq = GET_16BIT_NWO(packet, 6);
  NETDBG_PRINT( "-------- ICMP\n");
  NETDBG_PRINT( "Type:    %d", type);
  switch (type) {
  case 0: NETDBG_PRINT( " (Echo reply)\n"); break;
  case 3: NETDBG_PRINT( " (Destination unreachable)\n"); break;
  case 4: NETDBG_PRINT( " (Source quench)\n"); break;
  case 5: NETDBG_PRINT( " (Redirect)\n"); break;
  case 8: NETDBG_PRINT( " (Echo request)\n"); break;
  default: NETDBG_PRINT( " (Unknown)\n"); break;
  }
  NETDBG_PRINT( "Code:    %d\n", code);
  if (netdbg_checksum(packet, size)) {
    NETDBG_PRINT( "WARNING: Checksum failed\n");
  } else {
    NETDBG_PRINT( "Check:   Ok\n");
  }
  NETDBG_PRINT( "ID:      %u\n", id);
  NETDBG_PRINT( "Seqno:   %u\n", seq);
  if (type == 3 && size >= 28) {
    NETDBG_PRINT("-------- Encapsulated IPv4 header\n");
    netdbg_ipv4_header(packet + 8, size - 8);
  }
}
