#include "netdbg.h"

void netdbg_icmpv6
  (unsigned char* ipv6, unsigned char* icmp, unsigned size)
{
  unsigned type = icmp[0];
  unsigned code = icmp[1];
  _16BIT checksum = GET_16BIT_VAL(icmp, 2);
  NETDBG_PRINT( "-------- ICMPv6\n");
  NETDBG_PRINT( "Type:    %u ", type);
  switch (type) {
  case 1:   NETDBG_PRINT( "(Destination unreachable)\n"); break;
  case 2:   NETDBG_PRINT( "(Packet too big)\n"); break;
  case 3:   NETDBG_PRINT( "(Time exceeded)\n"); break;
  case 4:   NETDBG_PRINT( "(Parameter problem)\n"); break;
  case 128: NETDBG_PRINT( "(Echo request)\n"); break;
  case 129: NETDBG_PRINT( "(Echo reply)\n"); break;
  case 130: NETDBG_PRINT( "(MLD query)\n"); break;
  case 131: NETDBG_PRINT( "(MLD report)\n"); break;
  case 132: NETDBG_PRINT( "(MLD leave)\n"); break;
  case 133: NETDBG_PRINT( "(RDP solicitation)\n"); break;
  case 134: NETDBG_PRINT( "(RDP advertisement)\n"); break;
  case 135: NETDBG_PRINT( "(NDP solicitation)\n"); break;
  case 136: NETDBG_PRINT( "(NDP advertisement)\n"); break;
  case 143: NETDBG_PRINT( "(MLD report v.2)\n"); break;
  default:  NETDBG_PRINT( "(Unknown)\n"); break;
  }
  NETDBG_PRINT( "Code:    %u\n", code);
  NETDBG_PRINT( "Check:   %.4x\n", htons(checksum));
  if (ipv6) {
    _16BIT calcsum = 0;
    unsigned char pseudo[ 40 ];
    unsigned char orig[ 2 ] = { icmp[ 2 ], icmp[ 3 ] };
    icmp[ 2 ] = 0; icmp[ 3 ] = 0;
    memset(pseudo, 0, 40);
    memcpy(pseudo, ipv6 + 8, 16);
    memcpy(pseudo + 16, ipv6 + 24, 16);
    memcpy(pseudo + 34, ipv6 + 4, 2);
    pseudo[39] = 58;
    netdbg_crc16_feed(&calcsum, pseudo, 40);
    netdbg_crc16_feed(&calcsum, icmp, size);
    calcsum ^= 0xffff;
    if (calcsum != checksum) {
      NETDBG_PRINT( "WARNING: Checksum mismatch (%.4x)\n", htons(calcsum));
    }
    icmp[ 2 ] = orig[ 0 ];
    icmp[ 3 ] = orig[ 1 ];
  }
  switch (type) {
  case 1:    netdbg_icmpv6_destunreach(ipv6, icmp, size); break;
  case 2:    netdbg_icmpv6_packettoobig(ipv6, icmp, size); break;
  case 3:    netdbg_icmpv6_timeexceeded(ipv6, icmp, size); break;
  case 4:    netdbg_icmpv6_paramproblem(ipv6, icmp, size); break;
  case 128:  netdbg_icmpv6_echorequest(ipv6, icmp, size); break;
  case 129:  netdbg_icmpv6_echoreply(ipv6, icmp, size); break;
  case 130:  netdbg_icmpv6_mldquery(ipv6, icmp, size); break;
  case 131:  netdbg_icmpv6_mldreport(ipv6, icmp, size); break;
  case 132:  netdbg_icmpv6_mldleave(ipv6, icmp, size); break;
  case 133:  netdbg_icmpv6_rdpsol(ipv6, icmp, size); break;
  case 134:  netdbg_icmpv6_rdpadv(ipv6, icmp, size); break;
  case 135:  netdbg_icmpv6_ndpsol(ipv6, icmp, size); break;
  case 136:  netdbg_icmpv6_ndpadv(ipv6, icmp, size); break;
  case 143:  netdbg_icmpv6_mldreportv2(ipv6, icmp, size); break;
  default:   NETDBG_PRINT( "WARNING: ICMP type; no further processing.\n");
  }
}
