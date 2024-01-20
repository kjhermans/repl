/**
 * This file is part of Waffle.
 * Copyright 2018, Fox-IT BV Netherlands.
 * Written by KJ Hermans <hermans@fox-it.com>
 *
 * LICENSE:
 * Unless you have been explicitly granted the right, or unless you are a
 * Fox-IT employee, you have no license to use this file.
 *
 * \file
 * \brief
 */

#include "netdbg.h"

void netdbg_ple
  (unsigned char* ipv4, unsigned char* ple, unsigned size)
{
  unsigned keybit = (ple[ 0 ] >> 7);
  unsigned channel = (ple[ 0 ] & 0x7f);
  unsigned bootcount = ple[ 1 ];
  _16BIT peid = GET_16BIT_NWO(ple, 2);
  _32BIT counter = GET_32BIT_NWO(ple, 4);

  NETDBG_PRINT( "-------- PLE\n");
  NETDBG_PRINT("Keybit:  %u\n", keybit);
  NETDBG_PRINT("Channel: %u\n", channel);
  NETDBG_PRINT("Bootcnt: %u\n", bootcount);
  NETDBG_PRINT("PEID:    %u\n", peid);
  NETDBG_PRINT("Counter: %u\n", counter);
}
