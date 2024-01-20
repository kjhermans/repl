#include "netdbg.h"

void netdbg_ip_tcp
  (unsigned char* ipv4, unsigned char* ipv6, unsigned char* tcp, unsigned size)
{
  _16BIT source = GET_16BIT_NWO(tcp, 0);
  _16BIT dest = GET_16BIT_NWO(tcp, 2);
  _32BIT seqno = GET_32BIT_NWO(tcp, 4);
  _32BIT ackno = GET_32BIT_NWO(tcp, 8);
  int dataoff = (tcp[12] >> 4);
  int bits = tcp[13];
  _16BIT window = GET_16BIT_NWO(tcp, 14);
  _16BIT checksum = GET_16BIT_VAL(tcp, 16);
  _16BIT urg = GET_16BIT_NWO(tcp, 18);
  NETDBG_PRINT( "-------- TCP\n");
  NETDBG_PRINT( "S.port:  %u\n", source);
  NETDBG_PRINT( "D.port:  %u\n", dest);
  NETDBG_PRINT( "Seqno:   %u\n", seqno);
  NETDBG_PRINT( "Ackno:   %u\n", ackno);
  NETDBG_PRINT( "Dataoff: %u (actual %u, %u for options)\n",
    dataoff, dataoff * 4, (20 - (dataoff * 4))
  );
  NETDBG_PRINT( "Bits:    ");
  if ((bits >> 5) & 1) {
    NETDBG_PRINT( "URG ");
  }
  if ((bits >> 4) & 1) {
    NETDBG_PRINT( "ACK ");
  }
  if ((bits >> 3) & 1) {
    NETDBG_PRINT( "PSH ");
  }
  if ((bits >> 2) & 1) {
    NETDBG_PRINT( "RST ");
  }
  if ((bits >> 1) & 1) {
    NETDBG_PRINT( "SYN ");
  }
  if (bits & 1) {
    NETDBG_PRINT( "FIN ");
  }
  NETDBG_PRINT( "\n");
  NETDBG_PRINT( "Window:  %u\n", window);
  NETDBG_PRINT( "Chksum:  %.4x ", htons(checksum));
  //.. check checksum
  NETDBG_PRINT( "\n");
  NETDBG_PRINT( "Urgent:  %u\n", urg);
}

void netdbg_ipv4_tcp
  (unsigned char* ipv4, unsigned char* packet, unsigned size)
{
  netdbg_ip_tcp(ipv4, 0, packet, size);
}

void netdbg_ipv6_tcp
  (unsigned char* ipv6, unsigned char* packet, unsigned size)
{
  netdbg_ip_tcp(0, ipv6, packet, size);
}

void netdbg_tcp
  (unsigned char* tcp, unsigned size)
{
  netdbg_ip_tcp(0, 0, tcp, size);
}
