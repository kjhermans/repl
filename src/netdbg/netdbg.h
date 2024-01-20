#ifndef _NETDBG_H_
#define _NETDBG_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define _16BIT unsigned short
#define _32BIT unsigned int

#ifdef GET_16BIT_NWO
#undef GET_16BIT_NWO
#endif

#ifdef GET_16BIT_VAL
#undef GET_16BIT_VAL
#endif

#ifdef SET_16BIT_NWO
#undef SET_16BIT_NWO
#endif

#ifdef SET_16BIT_VAL
#undef SET_16BIT_VAL
#endif

#ifdef GET_32BIT_NWO
#undef GET_32BIT_NWO
#endif

#ifdef GET_32BIT_VAL
#undef GET_32BIT_VAL
#endif

#ifdef SET_32BIT_NWO
#undef SET_32BIT_NWO
#endif

#ifdef SET_32BIT_VAL
#undef SET_32BIT_VAL
#endif

#define GET_16BIT_NWO(buf,off) \
  ({ u_int16_t r; memcpy(&r, (buf)+(off), 2); r=htons(r); r; })
#define GET_16BIT_VAL(buf,off) \
  ({ u_int16_t r; memcpy(&r, (buf)+(off), 2); r; })
#define GET_32BIT_NWO(buf,off) \
  ({ u_int32_t r; memcpy(&r, (buf)+(off), 4); r=htonl(r); r; })
#define GET_32BIT_VAL(buf,off) \
  ({ u_int32_t r; memcpy(&r, (buf)+(off), 4); r; })
#define SET_16BIT_NWO(buf,off,val) \
  { u_int16_t r = htons(val); memcpy((buf)+(off), &r, 2); }
#define SET_16BIT_VAL(buf,off,val) \
  { memcpy((buf)+(off), &(val), 2); }
#define SET_32BIT_NWO(buf,off,val) \
  { u_int32_t r = htonl(val); memcpy((buf)+(off), &r, 4); }
#define SET_32BIT_VAL(buf,off,val) \
  { memcpy((buf)+(off), &(val), 4); }

extern _16BIT netdbg_checksum
  (unsigned char*, unsigned);
extern _16BIT netdbg_crc16_feed
  (_16BIT* state, unsigned char* data, unsigned int size);
extern _16BIT netdbg_crc16
  (unsigned char* data, unsigned int size);
extern void netdbg_ethernet
  (unsigned char*, unsigned);
extern void netdbg_ipv4
  (unsigned char*, unsigned);
extern void netdbg_ipv4_header
  (unsigned char*, unsigned);
extern void netdbg_ipv6
  (unsigned char*, unsigned);
extern void netdbg_arp
  (unsigned char*, unsigned);
extern void netdbg_ipv4_tcp
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_ipv6_tcp
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_tcp
  (unsigned char*, unsigned);
extern void netdbg_ipv4_udp
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_ipv6_udp
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_udp
  (unsigned char*, unsigned);
extern void netdbg_icmp
  (unsigned char*, unsigned);
extern void netdbg_icmpv6
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_destunreach
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_packettoobig
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_timeexceeded
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_paramproblem
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_echorequest
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_echoreply
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_mldquery
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_mldreport
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_mldleave
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_rdpsol
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_rdpadv
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_ndpsol
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_ndpadv
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_mldreportv2
  (unsigned char*, unsigned char*, unsigned);
extern void netdbg_icmpv6_ndpoptions
  (unsigned char* options, unsigned size);
extern void netdbg_igmp
  (unsigned char*, unsigned);

/** TCP conversation watching **/

struct message
  {
    struct timeval time;
    unsigned char* data;
    unsigned       size;
  };

struct ipv4tcpstate
  {
    _32BIT fromhost;
    _32BIT tohost;
    _16BIT fromport;
    _16BIT toport;
    struct message* messages;
    unsigned nmessages;
  };

extern struct ipv4tcpstate* netdbg_ipv4tcpstate_init
  (
    struct ipv4tcpstate* state,
    _32BIT fromhost,
    _32BIT tohost,
    _16BIT fromport,
    _16BIT toport
  );

extern int netdbg_ipv4tcpstate_feed_ethernet
  (struct ipv4tcpstate* state, unsigned char* packet, unsigned size);
extern int netdbg_ipv4tcpstate_feed_ipv4
  (struct ipv4tcpstate* state, unsigned char* packet, unsigned size);
extern int netdbg_ipv4tcpstate_feed_tcp
  (struct ipv4tcpstate* state, unsigned char* packet, unsigned size);

extern void netdbg_ipv4tcpstate_display
  (struct ipv4tcpstate* state);

extern void netdbg_ipv4_dhcp
  (unsigned char* dhcp, unsigned dhcpsize);

extern void netdbg_ple
  (unsigned char* ipv4, unsigned char* ple, unsigned size);

extern void NETDBG_PRINT
  (char* fmt, ...);

#endif
