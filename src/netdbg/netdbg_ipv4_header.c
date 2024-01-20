#include "netdbg.h"

static
char* ip4proto[] = {
  "TCP (0)",
  "ICMP",
  "IGMP",
  "TCP",
  "UDP",
  "Unknown"
};

static
char* netdbg_ipv4_protostr
  (int proto)
{
  switch (proto) {
  case 0:
    return ip4proto[0];
  case 1:
    return ip4proto[1];
  case 2:
    return ip4proto[2];
  case 6:
    return ip4proto[3];
  case 17:
    return ip4proto[4];
  default:
    return ip4proto[5];
  }
}

void netdbg_ipv4_header
  (unsigned char* packet, unsigned size)
{
  _16BIT checksum;
  unsigned ihl = (packet[0] & 0x0f);
  unsigned headerlength = ihl * 4;
  unsigned length = GET_16BIT_NWO(packet, 2);
  unsigned fragoff = packet[ 7 ]; fragoff |= ((packet[6] & 0x1f) << 8);

  NETDBG_PRINT( "-------- IPv4\n");
  if ((packet[0] >> 4) != 4) {
    NETDBG_PRINT( "WARNING: Packet doesn't seem to be IPv4.\n");
  }
  if ((checksum = netdbg_checksum(packet, headerlength)) != 0) {
    NETDBG_PRINT( "ERROR:   Checksum mismatch (%.4x).\n", checksum);
  } else {
    NETDBG_PRINT( "Checksum:Ok!\n");
  }
  if (length < size) {
    NETDBG_PRINT( "WARNING: Excess data (%u > %u); truncating.\n"
      , size, length
    );
    size = length;
  } else if (length != size) {
    NETDBG_PRINT( "WARNING: Packet length doesn't match given data size.\n");
  }
  NETDBG_PRINT( "IHL:     %u (%u bytes)\n", ihl, headerlength);
  if (size < headerlength) {
    NETDBG_PRINT( "Data given is too short for whole header.\n");
    return;
  }
  NETDBG_PRINT(
    "Length:  %u (%u for payload)\n", length, (length - (headerlength))
  );
  NETDBG_PRINT(
    "ID:      %u\n", GET_16BIT_NWO(packet, 4)
  );
  NETDBG_PRINT(
    "DF:      %u\n", ((packet[6] >> 6) & 0x01)
  );
  NETDBG_PRINT(
    "MF:      %u\n", ((packet[6] >> 5) & 0x01)
  );
  NETDBG_PRINT(
    "Fragoff: %u\n", fragoff
  );
  NETDBG_PRINT(
    "TTL:     %d\n", packet[8]
  );
  NETDBG_PRINT(
    "Proto:   %d (%s)\n", packet[9], netdbg_ipv4_protostr(packet[9])
  );
  NETDBG_PRINT(
    "Source:  %d.%d.%d.%d\n", packet[12], packet[13], packet[14], packet[15]
  );
  NETDBG_PRINT(
    "Dest.:   %d.%d.%d.%d\n", packet[16], packet[17], packet[18], packet[19]
  );
}
