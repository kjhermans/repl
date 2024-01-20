#include "netdbg.h"

int main
  (int argc, char* argv[])
{
  unsigned char packet0[] = {
    0x45, 0x00, 0x00, 0xb2, 0x4e, 0xa6, 0x20, 0x00,
    0x80, 0x06, 0xee, 0x3f, 0xc0, 0xa8, 0x48, 0x03,
    0xc0, 0xa8, 0x48, 0x04, 0x04, 0x4c, 0x00, 0x8b,
    0x00, 0xe5, 0xc3, 0xa2, 0x43, 0xc1, 0xcc, 0x20,
    0x50, 0x18, 0x21, 0x7f, 0xf6, 0xf9, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x86, 0xff, 0x53, 0x4d, 0x42,
    0x32, 0x00, 0x00, 0x00, 0x00, 0x18, 0x03, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xfe, 0xca,
    0x00, 0x48, 0x42, 0xf1, 0x0f, 0x42, 0x00, 0x00
  };
  unsigned char packet1[] = {
    0x45, 0x00, 0x00, 0x6c,
    0x92, 0xcc, 0x00, 0x00,
    0x38, 0x06, 0xe4, 0x04,
    0x92, 0x95, 0xba, 0x14,
    0xa9, 0x7c, 0x15, 0x95
  };
  netdbg_ipv4(packet0, sizeof(packet0));
  netdbg_ipv4(packet1, sizeof(packet1));
  return WFOK;
}