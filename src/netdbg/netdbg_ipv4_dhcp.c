/*
 * Copyright(c) 2015, 2016 Fox-IT BV
 * Written by Kees-Jan Hermans <hermans-it.com>
 * License: Exclusive use by Fox-IT
 */

#include "netdbg.h"

#define DHCP_OPTIONTYPE_SUBNETMASK   0x01
#define DHCP_OPTIONTYPE_ROUTER       0x03
#define DHCP_OPTIONTYPE_TIMED        0x04
#define DHCP_OPTIONTYPE_NAMED        0x05
#define DHCP_OPTIONTYPE_DNSD         0x06
#define DHCP_OPTIONTYPE_LPRD         0x09
#define DHCP_OPTIONTYPE_HOSTNAME     0x0c
#define DHCP_OPTIONTYPE_DOMAINNAME   0x0f
#define DHCP_OPTIONTYPE_MTU          0x1a
#define DHCP_OPTIONTYPE_BROADCAST    0x1c
#define DHCP_OPTIONTYPE_NTPD         0x2a
#define DHCP_OPTIONTYPE_REQUESTEDIP  0x32
#define DHCP_OPTIONTYPE_LEASETIME    0x33
#define DHCP_OPTIONTYPE_MSGTYPE      0x35
#define DHCP_OPTIONTYPE_SERVERIDENT  0x36
#define DHCP_OPTIONTYPE_REQPARAMS    0x37
#define DHCP_OPTIONTYPE_RENEWTIME    0x3a
#define DHCP_OPTIONTYPE_REBINDTIME   0x3b
#define DHCP_OPTIONTYPE_CLIENTIDENT  0x3d
#define DHCP_OPTIONTYPE_END          0xff

void netdbg_ipv4_dhcp
  (unsigned char* dhcp, unsigned dhcpsize)
{
  NETDBG_PRINT( "-------- BOOTP (or DHCP)\n");
  if (dhcpsize < 236) {
    NETDBG_PRINT( "ERROR:   Message length too small (%u)\n", dhcpsize);
    return;
  }
  switch (dhcp[ 0 ]) {
  case 1: NETDBG_PRINT( "Op:      bootrequest\n"); break;
  case 2: NETDBG_PRINT( "Op:      bootreply\n"); break;
  default: NETDBG_PRINT( "Op:      unknown (%u)\n", dhcp[ 0 ]); break;
  }
  NETDBG_PRINT( "Htype:   %u\n", dhcp[ 1 ]);
  NETDBG_PRINT( "Hlen:    %u\n", dhcp[ 2 ]);
  NETDBG_PRINT( "Hops:    %u\n", dhcp[ 3 ]);
  NETDBG_PRINT( "XID:     %u\n", GET_32BIT_NWO(dhcp, 4));
  NETDBG_PRINT( "Secs:    %u\n", GET_16BIT_NWO(dhcp, 8));
  NETDBG_PRINT( "Flags:   %u\n", GET_16BIT_NWO(dhcp, 10));
  NETDBG_PRINT( "CIADDR:  %u.%u.%u.%u\n",
    dhcp[ 12 ], dhcp[ 13 ], dhcp[ 14 ], dhcp[ 15 ]
  );
  NETDBG_PRINT( "YIADDR:  %u.%u.%u.%u\n",
    dhcp[ 16 ], dhcp[ 17 ], dhcp[ 18 ], dhcp[ 19 ]
  );
  NETDBG_PRINT( "SIADDR:  %u.%u.%u.%u\n",
    dhcp[ 20 ], dhcp[ 21 ], dhcp[ 22 ], dhcp[ 23 ]
  );
  NETDBG_PRINT( "GIADDR:  %u.%u.%u.%u\n",
    dhcp[ 24 ], dhcp[ 25 ], dhcp[ 26 ], dhcp[ 27 ]
  );
  if (dhcpsize > 240
      && dhcp[ 236 ] == 0x63
      && dhcp[ 237 ] == 0x82
      && dhcp[ 238 ] == 0x53
      && dhcp[ 239 ] == 0x63)
  {
    unsigned char* option = dhcp + 240;
    unsigned options_size = dhcpsize - 240;
    while (options_size >= 2) {
      unsigned optiontype = option[ 0 ];
      unsigned optionsize = option[ 1 ];
      if (options_size < optionsize + 2) {
        NETDBG_PRINT( "ERROR:   Option size too small (%u < %u)\n",
          options_size, optionsize + 2
        );
        return;
      }
      switch (optiontype) {
      case 0x01:
        NETDBG_PRINT( "Option:  Subnet mask ");
        if (optionsize == 4) {
          NETDBG_PRINT( "%u.%u.%u.%u\n",
            option[ 2 ], option[ 3 ], option[ 4 ], option[ 5 ]
          );
        } else {
          NETDBG_PRINT( "WARNING: Type length should be 4\n");
        }
        break;
      case 0x32:
        NETDBG_PRINT( "Option:  Requested IP ");
        if (optionsize == 4) {
          NETDBG_PRINT( "%u.%u.%u.%u\n",
            option[ 2 ], option[ 3 ], option[ 4 ], option[ 5 ]
          );
        } else {
          NETDBG_PRINT( "WARNING: Type length should be 4\n");
        }
        break;
      case 0x33:
        NETDBG_PRINT( "Option:  Address time ");
        if (optionsize == 4) {
          NETDBG_PRINT( "%u\n", GET_32BIT_NWO(option, 2));
        } else {
          NETDBG_PRINT( "WARNING: Type length should be 4\n");
        }
        break;
      case 0x35:
        NETDBG_PRINT( "Option:  Type:");
        if (optionsize == 1) {
          switch (option[ 2 ]) {
          case 0x01: NETDBG_PRINT( "DISCOVER\n"); break;
          case 0x02: NETDBG_PRINT( "OFFER\n"); break;
          case 0x03: NETDBG_PRINT( "REQUEST\n"); break;
          case 0x04: NETDBG_PRINT( "DECLINE\n"); break;
          case 0x05: NETDBG_PRINT( "ACK\n"); break;
          case 0x06: NETDBG_PRINT( "NAK\n"); break;
          case 0x07: NETDBG_PRINT( "RELEASE\n"); break;
          case 0x08: NETDBG_PRINT( "INFORM\n"); break;
          default: NETDBG_PRINT( "unknown\n"); break;
          }
        } else {
          NETDBG_PRINT( "WARNING: Type length should be 1\n");
        }
        break;
      case 0x36:
        NETDBG_PRINT( "Option:  Server ID ");
        if (optionsize == 4) {
          NETDBG_PRINT( "%u.%u.%u.%u\n",
            option[ 2 ], option[ 3 ], option[ 4 ], option[ 5 ]
          );
        } else {
          NETDBG_PRINT( "WARNING: Server ID should be 4\n");
        }
        break;
      case 0x37:
        NETDBG_PRINT( "Option:  Parameter request list: ");
        {
          unsigned i;
          for (i=2; i < optionsize; i++) {
            switch (option[ i ]) {
            case DHCP_OPTIONTYPE_SUBNETMASK: NETDBG_PRINT( "SUBNETMASK ");
              break;
            case DHCP_OPTIONTYPE_ROUTER: NETDBG_PRINT( "ROUTER ");
              break;
            case DHCP_OPTIONTYPE_TIMED: NETDBG_PRINT( "TIMED ");
              break;
            case DHCP_OPTIONTYPE_NAMED: NETDBG_PRINT( "NAMED ");
              break;
            case DHCP_OPTIONTYPE_DNSD: NETDBG_PRINT( "DNSD ");
              break;
            case DHCP_OPTIONTYPE_LPRD: NETDBG_PRINT( "LPRD ");
              break;
            case DHCP_OPTIONTYPE_HOSTNAME: NETDBG_PRINT( "HOSTNAME ");
              break;
            case DHCP_OPTIONTYPE_DOMAINNAME: NETDBG_PRINT( "DOMAINNAME ");
              break;
            case DHCP_OPTIONTYPE_MTU: NETDBG_PRINT( "MTU ");
              break;
            case DHCP_OPTIONTYPE_BROADCAST: NETDBG_PRINT( "BROADCAST ");
              break;
            case DHCP_OPTIONTYPE_NTPD: NETDBG_PRINT( "NTPD ");
              break;
            case DHCP_OPTIONTYPE_REQUESTEDIP: NETDBG_PRINT( "REQUESTEDIP ");
              break;
            case DHCP_OPTIONTYPE_LEASETIME: NETDBG_PRINT( "LEASETIME ");
              break;
            case DHCP_OPTIONTYPE_MSGTYPE: NETDBG_PRINT( "MSGTYPE ");
              break;
            case DHCP_OPTIONTYPE_SERVERIDENT: NETDBG_PRINT( "SERVERIDENT ");
              break;
            case DHCP_OPTIONTYPE_REQPARAMS: NETDBG_PRINT( "REQPARAMS ");
              break;
            case DHCP_OPTIONTYPE_RENEWTIME: NETDBG_PRINT( "RENEWTIME ");
              break;
            case DHCP_OPTIONTYPE_REBINDTIME: NETDBG_PRINT( "REBINDTIME ");
              break;
            case DHCP_OPTIONTYPE_CLIENTIDENT: NETDBG_PRINT( "CLIENTIDENT ");
              break;
            case DHCP_OPTIONTYPE_END: NETDBG_PRINT( "END ");
              break;
            default: NETDBG_PRINT( "unknown ");
              break;
            }
          }
          NETDBG_PRINT( "\n");
        }
        break;
      case 0xff:
        NETDBG_PRINT( "Option:  End of options.\n");
        return;
      default:
        NETDBG_PRINT( "Option:  unknown (%u)\n", optiontype);
        break;
      }
      option += (optionsize + 2);
      options_size -= (optionsize + 2);
    }
  }
}
