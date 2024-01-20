#include "netdbg.h"

void netdbg_ip_udp
  (unsigned char* ipv4, unsigned char* ipv6, unsigned char* udp, unsigned size)
{
  _16BIT source = GET_16BIT_NWO(udp, 0);
  _16BIT dest = GET_16BIT_NWO(udp, 2);
  _16BIT length = GET_16BIT_NWO(udp, 4);
  _16BIT checksum = GET_16BIT_VAL(udp, 6);

  if (size < 8) {
    NETDBG_PRINT( "ERROR:   UDP size is smaller than allowed.\n");
    return;
  }
  NETDBG_PRINT( "-------- UDP\n");
  NETDBG_PRINT( "S.port:  %u\n", source);
  NETDBG_PRINT( "D.port:  %u\n", dest);
  NETDBG_PRINT( "Length:  %u\n", length);
  if (length < 8) {
    NETDBG_PRINT( "WARNING: Length is smaller than allowed.\n");
  }
  if (checksum == 0) {
    NETDBG_PRINT( "WARNING: Checksum is zero.\n");
  } else if (ipv4 && size >= length) {
    _16BIT _checksum;
    unsigned char pseudopacket[ 12 + length ];
    memcpy(pseudopacket, ipv4 + 12, 4);
    memcpy(pseudopacket + 4, ipv4 + 16, 4);
    pseudopacket[8] = 0;
    pseudopacket[9] = 17;
    memcpy(pseudopacket + 10, udp + 4, 2);
    memcpy(pseudopacket + 12, udp, length);
    pseudopacket[18] = 0;
    pseudopacket[19] = 0;
    _checksum = netdbg_checksum(pseudopacket, 12 + length);
    if (_checksum != checksum) {
      NETDBG_PRINT(
        "Check:   Failed (got %.4x, should be %.4x)\n",
        htons(checksum), htons(_checksum)
      );
    } else {
      NETDBG_PRINT( "Check:   Ok\n");
    }
  } else if (ipv6 && size >= length) {
    //..
  } else {
    NETDBG_PRINT( "WARNING: Cannot check checksum (data missing).\n");
  }
  if (source == 68 && dest == 67) {
    NETDBG_PRINT( "Proto:   BOOTP (or DHCP) request\n");
    if (ipv4) {
      netdbg_ipv4_dhcp(udp + 8, size - 8);
    } // else if IPv6
  } else if (source == 67 && dest == 68) {
    NETDBG_PRINT("Proto:   BOOTP (or DHCP) reply\n");
    if (ipv4) {
      netdbg_ipv4_dhcp(udp + 8, size - 8);
    } // else if IPv6
  } else if (dest == 514 || dest == 1514 || dest == 5199) {
    NETDBG_PRINT("Cont : %-.*s\n", size - 8, udp + 8);
  }
}

void netdbg_ipv4_udp
  (unsigned char* ipv4, unsigned char* udp, unsigned size)
{
  netdbg_ip_udp(ipv4, 0, udp, size);
}

void netdbg_ipv6_udp
  (unsigned char* ipv6, unsigned char* udp, unsigned size)
{
  netdbg_ip_udp(0, ipv6, udp, size);
}

void netdbg_udp
  (unsigned char* packet, unsigned size)
{
  netdbg_ipv4_udp(0, packet, size);
}
