#include "netdbg.h"

/*
_16BIT netdbg_checksum
  (unsigned char* data, unsigned size)
{
  unsigned long overflow = 0x10000;
  _16BIT result = 0;
  while (size >= 2) {
    unsigned long intermediate = ntohs(result) + GET_16BIT_NWO(data, 0);
    if (intermediate & overflow) {
      intermediate &= 0xffff;
      ++intermediate;
    }
    result = ntohs((_16BIT)intermediate);
    data += 2;
    size -= 2;
  }
  return (result ^ 0xffff);
}
*/

/**
 * WARNING: When state is an un-aligned pointer, it may
 * fail to perform as expected on ARM.
 */
_16BIT netdbg_crc16_feed
  (_16BIT* state, unsigned char* data, unsigned int size)
{
  static const unsigned long overflowbit = (1<<16);
  static const unsigned long bitsallowed = 0xffff;
  unsigned long reg = (unsigned long)ntohs(*state);

  while (size) {
    _16BIT word;
    if (size == 1) {
      word = (_16BIT)(data[0]);
    } else {
      word = GET_16BIT_VAL(data, 0);
    }
    word = ntohs(word);
    reg += word;
    if (reg & overflowbit) {
      ++reg;
      reg &= bitsallowed;
    }
    if (size == 1) {
      break;
    }
    size -= sizeof(_16BIT);
    data += sizeof(_16BIT);
  }
  *state = htons((_16BIT)reg);
  return *state;
}

_16BIT netdbg_crc16
  (unsigned char* data, unsigned int size)
{
  _16BIT checksum = 0;
  return netdbg_crc16_feed(&checksum, data, size);
}

_16BIT netdbg_checksum
  (unsigned char* data, unsigned int size)
{
  return (netdbg_crc16(data, size) ^ 0xffff);
}
