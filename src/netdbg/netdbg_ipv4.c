#include "netdbg.h"

/*
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
*/

void netdbg_ipv4
  (unsigned char* packet, unsigned size)
{
  unsigned ihl = (packet[0] & 0x0f);
  unsigned headerlength = ihl * 4;

  netdbg_ipv4_header(packet, size);
  if (packet[ 6 ] & 0x1f || packet[ 7 ]) {
    NETDBG_PRINT( "Packet is (further) fragment (no analysis)\n");
  } else {
    switch (packet[9]) {
    case 0:
    case 6:
      if (size - headerlength >= 20) {
        netdbg_ipv4_tcp(packet, packet + headerlength, size - headerlength);
      } else {
        NETDBG_PRINT( "WARNING: Not enough data to analyze TCP header.\n");
      }
      break;
    case 1:
      if (size - headerlength >= 8) {
        netdbg_icmp(packet + headerlength, size - headerlength);
      } else {
        NETDBG_PRINT( "WARNING: Not enough data to analyze ICMP header.\n");
      }
      break;
    case 2:
      if (size - headerlength >= 4) {
        netdbg_igmp(packet + headerlength, size - headerlength);
      } else {
        NETDBG_PRINT( "WARNING: Not enough data to analyze IGMP header.\n");
      }
      break;
    case 17:
      if (size - headerlength >= 8) {
        netdbg_ipv4_udp(packet, packet + headerlength, size - headerlength);
      } else {
        NETDBG_PRINT( "WARNING: Not enough data to analyze UDP header.\n");
      }
      break;
    case 99:
      if (size - headerlength >= 8) {
        netdbg_ple(packet, packet + headerlength, size - headerlength);
      } else {
        NETDBG_PRINT( "WARNING: Not enough data to analyze PLE header.\n");
      }
    default:
      NETDBG_PRINT( "--------\n");
      break;
    }
  }
}
