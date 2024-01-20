#include "netdbg.h"

static
char* modestr[] = {
  0,
  "MODE_IS_INCLUDE",
  "MODE_IS_EXCLUDE",
  "CHANGE_TO_INCLUDE",
  "CHANGE_TO_EXCLUDE"
};

void netdbg_icmpv6_mldreportv2
  (unsigned char* ipv6, unsigned char* icmp, unsigned size)
{
  unsigned char* record = icmp + 8;
  unsigned i, nrecords = GET_16BIT_NWO(icmp, 6);
  NETDBG_PRINT( "-------- ICMPv6 MLDv2 Report\n");
  NETDBG_PRINT( "NRecords %u\n", nrecords);
  size -= 8;
  for (i=0; i < nrecords; i++) {
    unsigned recsize, j, nsources = GET_16BIT_NWO(record, 2);
    NETDBG_PRINT( "-------- ICMPv6 MLDv2 Report Record %u\n", i);
    NETDBG_PRINT( "RecType: %u\n", record[0]);
    NETDBG_PRINT( "RecStr:  %s\n",
      ((record[0] >= 1 && record[0] <= 4) ? modestr[ record[0] ] : "Unknown")
    );
    NETDBG_PRINT( "AuxDatLn %u\n", record[1]);
    NETDBG_PRINT( "NSources %u\n", nsources);
    NETDBG_PRINT( "Address: %x:%x:%x:%x:%x:%x:%x:%x\n",
      GET_16BIT_NWO(record, 4),
      GET_16BIT_NWO(record, 6),
      GET_16BIT_NWO(record, 8),
      GET_16BIT_NWO(record, 10),
      GET_16BIT_NWO(record, 12),
      GET_16BIT_NWO(record, 14),
      GET_16BIT_NWO(record, 16),
      GET_16BIT_NWO(record, 18)
    );
    recsize = 20 + (nsources * 16);
    if (recsize > size) {
      NETDBG_PRINT( "WARNING: Overflow.\n");
      return;
    }
    for (j=0; j < nsources; j++) {
      NETDBG_PRINT( "Src %.4u %x:%x:%x:%x:%x:%x:%x:%x\n", j,
        GET_16BIT_NWO(record, 20 + (j * 16) + 0),
        GET_16BIT_NWO(record, 20 + (j * 16) + 2),
        GET_16BIT_NWO(record, 20 + (j * 16) + 4),
        GET_16BIT_NWO(record, 20 + (j * 16) + 6),
        GET_16BIT_NWO(record, 20 + (j * 16) + 8),
        GET_16BIT_NWO(record, 20 + (j * 16) + 10),
        GET_16BIT_NWO(record, 20 + (j * 16) + 12),
        GET_16BIT_NWO(record, 20 + (j * 16) + 14)
      );
    }
    size -= recsize;
    record += recsize;
  }
}
