#include "netdbg.h"

static
void netdbg_igmp_query
  (unsigned char* packet, unsigned size)
{
  int maxrespcode = packet[1];
  _16BIT checksum = GET_16BIT_NWO(packet, 2);
  NETDBG_PRINT( "Type:    Query\n");
  if (size >= 12) {
    NETDBG_PRINT( "Version: 3\n");
  } else {
    NETDBG_PRINT( "Version: 1 or 2\n");
  }
  NETDBG_PRINT( "Maxresp: %d", maxrespcode);
  if (maxrespcode & 0x80) {
    int mant = maxrespcode & 0x0f;
    int exp = (maxrespcode >> 4) & 0x07;
    NETDBG_PRINT( " (%d seconds)\n", (mant | 0x10) << (exp + 3));
  } else {
    NETDBG_PRINT( "\n");
  }
  NETDBG_PRINT( "Check:   %.4x\n", htons(checksum));
  if (netdbg_checksum(packet, size)) {
    NETDBG_PRINT( "ERROR:   Checksum mismatch\n");
  }
  NETDBG_PRINT( "Address: %d.%d.%d.%d\n",
    packet[4], packet[5], packet[6], packet[7]
  );
  if (size >= 12) {
    unsigned off = 12;
    _16BIT nsources = GET_16BIT_NWO(packet, 10);
    NETDBG_PRINT( "nsrc.:   %u\n", nsources);
    while (off <= size - 4) {
      NETDBG_PRINT( "Source:  %d.%d.%d.%d\n",
        packet[off], packet[off+1], packet[off+2], packet[off+3]
      );
      off += 4;
    }
  }
}

static
void netdbg_igmp_reportv3
  (unsigned char* packet, unsigned size)
{
  _16BIT checksum = GET_16BIT_VAL(packet, 2);
  _16BIT nrecords = GET_16BIT_NWO(packet, 6);
  NETDBG_PRINT( "Type:    Version 3 report\n");
  NETDBG_PRINT( "Check:   %.4x\n", htons(checksum));
  if (netdbg_checksum(packet, size)) {
    NETDBG_PRINT( "ERROR:   Checksum mismatch\n");
  }
  NETDBG_PRINT( "nrecs.:  %u\n", nrecords);
  //..
}

static
void netdbg_igmp_reportv1
  (unsigned char* packet, unsigned size)
{
  _16BIT checksum = GET_16BIT_VAL(packet, 2);
  NETDBG_PRINT( "Type:    Version 1 report\n");
  NETDBG_PRINT( "Check:   %.4x\n", htons(checksum));
  if (netdbg_checksum(packet, size)) {
    NETDBG_PRINT( "ERROR:   Checksum mismatch\n");
  }
  NETDBG_PRINT( "Address: %d.%d.%d.%d\n",
    packet[4], packet[5], packet[6], packet[7]
  );
}

static
void netdbg_igmp_reportv2
  (unsigned char* packet, unsigned size)
{
  _16BIT checksum = GET_16BIT_VAL(packet, 2);
  NETDBG_PRINT( "Type:    Version 2 report\n");
  NETDBG_PRINT( "Check:   %.4x\n", htons(checksum));
  if (netdbg_checksum(packet, size)) {
    NETDBG_PRINT( "ERROR:   Checksum mismatch\n");
  }
  NETDBG_PRINT( "Address: %d.%d.%d.%d\n",
    packet[4], packet[5], packet[6], packet[7]
  );
}

static
void netdbg_igmp_leavev2
  (unsigned char* packet, unsigned size)
{
  _16BIT checksum = GET_16BIT_VAL(packet, 2);
  NETDBG_PRINT( "Type:    Version 2 leave\n");
  NETDBG_PRINT( "Check:   %.4x\n", htons(checksum));
  if (netdbg_checksum(packet, size)) {
    NETDBG_PRINT( "ERROR:   Checksum mismatch\n");
  }
  NETDBG_PRINT( "Address: %d.%d.%d.%d\n",
    packet[4], packet[5], packet[6], packet[7]
  );
}

void netdbg_igmp
  (unsigned char* packet, unsigned size)
{
  NETDBG_PRINT( "-------- IGMP\n");
  switch (packet[0]) {
  case 0x11:
    netdbg_igmp_query(packet, size);
    break;
  case 0x22:
    netdbg_igmp_reportv3(packet, size);
    break;
  case 0x12:
    netdbg_igmp_reportv1(packet, size);
    break;
  case 0x16:
    netdbg_igmp_reportv2(packet, size);
    break;
  case 0x17:
    netdbg_igmp_leavev2(packet, size);
    break;
  default:
    NETDBG_PRINT( "Type:    Unknown\n");
    break;
  }
}
