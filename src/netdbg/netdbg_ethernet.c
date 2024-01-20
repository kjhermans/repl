#include "netdbg.h"

#include <sys/time.h>

static
char* ethertypes[] = {
  "IPv4",
  "IPv6",
  "ARP",
  "Unknown"
};

char* netdbg_ethertype
  (unsigned char* type)
{
  if (type[0] == 0x08 && type[1] == 0x00) {
    return ethertypes[0];
  } else if (type[0] == 0x86 && type[1] == 0xdd) {
    return ethertypes[1];
  } else if (type[0] == 0x08 && type[1] == 0x06) {
    return ethertypes[2];
  } else {
    return ethertypes[3];
  }
}

void netdbg_ethernet
  (unsigned char* frame, unsigned size)
{
  struct timeval tv;

  gettimeofday(&tv, 0);
  NETDBG_PRINT( "%d.%u\n", (int)(tv.tv_sec), (unsigned)(tv.tv_usec));
  if (size < 14) {
    NETDBG_PRINT( "Size (%u) too small for an ethernet frame.\n", size);
  }
  NETDBG_PRINT(
    "-------- Ethernet\n"
    "Dest:    %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n"
    "Source:  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n"
    "Type:    %.2x%.2x (%s)\n"
    , frame[0], frame[1], frame[2], frame[3], frame[4], frame[5]
    , frame[6], frame[7], frame[8], frame[9], frame[10], frame[11]
    , frame[12], frame[13], netdbg_ethertype(frame + 12)
  );
  if (frame[12] == 0x08 && frame[13] == 0x00) {
    if (size >= 34) {
      netdbg_ipv4(frame + 14, size - 14);
    } else {
      NETDBG_PRINT( "No further processing.\n");
    }
  } else if (frame[12] == 0x86 && frame[13] == 0xdd) {
    if (size >= 54) {
      netdbg_ipv6(frame + 14, size - 14);
    } else {
      NETDBG_PRINT( "No further processing.\n");
    }
  } else if (frame[12] == 0x08 && frame[13] == 0x06) {
    if (size >= 42) {
      netdbg_arp(frame + 14, size - 14);
    } else {
      NETDBG_PRINT( "No further processing.\n");
    }
  } else {
    NETDBG_PRINT( "----\n");
  }
}
