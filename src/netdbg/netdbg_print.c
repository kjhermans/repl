/*
 * Copyright(c) 2015, 2016 Fox-IT BV
 * Written by Kees-Jan Hermans <hermans-it.com>
 * License: Exclusive use by Fox-IT
 */

#include <stdarg.h>

#include "netdbg.h"

/**
 *
 */
void NETDBG_PRINT
  (char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
}
