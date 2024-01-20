#include "netdbg.h"

struct ipv6protos
{
  u_int8_t proto;
  char*    name;
} ipv6protos[] = {
  { 0 	, "HOPOPT 	IPv6 Hop-by-Hop Option 	RFC 2460" },
  { 1 	, "ICMP 	Internet Control Message Protocol 	RFC 792" },
  { 2 	, "IGMP 	Internet Group Management Protocol 	RFC 1112" },
  { 3 	, "GGP 	Gateway-to-Gateway Protocol 	RFC 823" },
  { 4 	, "IP-in-IP 	IP in IP (encapsulation) 	RFC 2003" },
  { 5 	, "ST 	Internet Stream Protocol 	RFC 1190, RFC 1819" },
  { 6 	, "TCP 	Transmission Control Protocol 	RFC 793" },
  { 7 	, "CBT 	Core-based trees 	RFC 2189" },
  { 8 	, "EGP 	Exterior Gateway Protocol 	RFC 888" },
  { 9 	, "IGP 	Interior Gateway Protocol (any private interior gateway (used by Cisco for their IGRP)) 	" },
  { 10 	, "BBN-RCC-MON 	BBN RCC Monitoring 	" },
  { 11 	, "NVP-II 	Network Voice Protocol 	RFC 741" },
  { 12 	, "PUP 	Xerox PUP 	" },
  { 13 	, "ARGUS 	ARGUS 	" },
  { 14 	, "EMCON 	EMCON 	" },
  { 15 	, "XNET 	Cross Net Debugger 	IEN 158" },
  { 16 	, "CHAOS 	Chaos 	" },
  { 17 	, "UDP 	User Datagram Protocol 	RFC 768" },
  { 18 	, "MUX 	Multiplexing 	IEN 90" },
  { 19 	, "DCN-MEAS 	DCN Measurement Subsystems 	" },
  { 20 	, "HMP 	Host Monitoring Protocol 	RFC 869" },
  { 21 	, "PRM 	Packet Radio Measurement 	" },
  { 22 	, "XNS-IDP 	XEROX NS IDP 	" },
  { 23 	, "TRUNK-1 	Trunk-1 	" },
  { 24 	, "TRUNK-2 	Trunk-2 	" },
  { 25 	, "LEAF-1 	Leaf-1 	" },
  { 26 	, "LEAF-2 	Leaf-2 	" },
  { 27 	, "RDP 	Reliable Datagram Protocol 	RFC 908" },
  { 28 	, "IRTP 	Internet Reliable Transaction Protocol 	RFC 938" },
  { 29 	, "ISO-TP4 	ISO Transport Protocol Class 4 	RFC 905" },
  { 30 	, "NETBLT 	Bulk Data Transfer Protocol 	RFC 998" },
  { 31 	, "MFE-NSP 	MFE Network Services Protocol 	" },
  { 32 	, "MERIT-INP 	MERIT Internodal Protocol 	" },
  { 33 	, "DCCP 	Datagram Congestion Control Protocol 	RFC 4340" },
  { 34 	, "PC 	Third Party Connect Protocol 	" },
  { 35 	, "IDPR 	Inter-Domain Policy Routing Protocol 	RFC 1479" },
  { 36 	, "XTP 	Xpress Transport Protocol 	" },
  { 37 	, "DDP 	Datagram Delivery Protocol 	" },
  { 38 	, "IDPR-CMTP 	IDPR Control Message Transport Protocol 	" },
  { 39 	, "TP++ 	TP++ Transport Protocol 	" },
  { 40 	, "IL 	IL Transport Protocol 	" },
  { 41 	, "IPv6 	IPv6 Encapsulation 	RFC 2473" },
  { 42 	, "SDRP 	Source Demand Routing Protocol 	RFC 1940" },
  { 43 	, "IPv6-Route 	Routing Header for IPv6 	RFC 2460" },
  { 44 	, "IPv6-Frag 	Fragment Header for IPv6 	RFC 2460" },
  { 45 	, "IDRP 	Inter-Domain Routing Protocol 	" },
  { 46 	, "RSVP 	Resource Reservation Protocol 	RFC 2205" },
  { 47 	, "GRE 	Generic Routing Encapsulation 	RFC 2784, RFC 2890" },
  { 48 	, "MHRP 	Mobile Host Routing Protocol 	" },
  { 49 	, "BNA 	BNA 	" },
  { 50 	, "ESP 	Encapsulating Security Payload 	RFC 4303" },
  { 51 	, "AH 	Authentication Header 	RFC 4302" },
  { 52 	, "I-NLSP 	Integrated Net Layer Security Protocol 	TUBA" },
  { 53 	, "SWIPE 	SwIPe 	IP with Encryption" },
  { 54 	, "NARP 	NBMA Address Resolution Protocol 	RFC 1735" },
  { 55 	, "MOBILE 	IP Mobility (Min Encap) 	RFC 2004" },
  { 56 	, "TLSP 	Transport Layer Security Protocol (using Kryptonet key management) 	" },
  { 57 	, "SKIP 	Simple Key-Management for Internet Protocol 	RFC 2356" },
  { 58 	, "IPv6-ICMP 	ICMP for IPv6 	RFC 4443, RFC 4884" },
  { 59 	, "IPv6-NoNxt 	No Next Header for IPv6 	RFC 2460" },
  { 60 	, "IPv6-Opts 	Destination Options for IPv6 	RFC 2460" },
  { 61 	, "Any host internal protocol 	" },
  { 62 	, "CFTP 	CFTP 	" },
  { 63 	, "Any local network 	" },
  { 64 	, "SAT-EXPAK 	SATNET and Backroom EXPAK 	" },
  { 65 	, "KRYPTOLAN 	Kryptolan 	" },
  { 66 	, "RVD 	MIT Remote Virtual Disk Protocol 	" },
  { 67 	, "IPPC 	Internet Pluribus Packet Core 	" },
  { 68 	, "Any distributed file system 	" },
  { 69 	, "SAT-MON 	SATNET Monitoring 	" },
  { 70 	, "VISA 	VISA Protocol 	" },
  { 71 	, "IPCU 	Internet Packet Core Utility 	" },
  { 72 	, "CPNX 	Computer Protocol Network Executive 	" },
  { 73 	, "CPHB 	Computer Protocol Heart Beat 	" },
  { 74 	, "WSN 	Wang Span Network 	" },
  { 75 	, "PVP 	Packet Video Protocol 	" },
  { 76 	, "BR-SAT-MON 	Backroom SATNET Monitoring 	" },
  { 77 	, "SUN-ND 	SUN ND PROTOCOL-Temporary 	" },
  { 78 	, "WB-MON 	WIDEBAND Monitoring 	" },
  { 79 	, "WB-EXPAK 	WIDEBAND EXPAK 	" },
  { 80 	, "ISO-IP 	International Organization for Standardization Internet Protocol 	" },
  { 81 	, "VMTP 	Versatile Message Transaction Protocol 	RFC 1045" },
  { 82 	, "SECURE-VMTP 	Secure Versatile Message Transaction Protocol 	RFC 1045" },
  { 83 	, "VINES 	VINES 	" },
  { 84 	, "TTP 	TTP 	" },
  { 84 	, "IPTM 	Internet Protocol Traffic Manager 	" },
  { 85 	, "NSFNET-IGP 	NSFNET-IGP 	" },
  { 86 	, "DGP 	Dissimilar Gateway Protocol 	" },
  { 87 	, "TCF 	TCF 	" },
  { 88 	, "EIGRP 	EIGRP 	" },
  { 89 	, "OSPF 	Open Shortest Path First 	RFC 1583" },
  { 90 	, "Sprite-RPC 	Sprite RPC Protocol 	" },
  { 91 	, "LARP 	Locus Address Resolution Protocol 	" },
  { 92 	, "MTP 	Multicast Transport Protocol 	" },
  { 93 	, "AX.25 	AX.25 	" },
  { 94 	, "IPIP 	IP-within-IP Encapsulation Protocol 	RFC 2003" },
  { 95 	, "MICP 	Mobile Internetworking Control Protocol 	" },
  { 96 	, "SCC-SP 	Semaphore Communications Sec. Pro 	" },
  { 97 	, "ETHERIP 	Ethernet-within-IP Encapsulation 	RFC 3378" },
  { 98 	, "ENCAP 	Encapsulation Header 	RFC 1241" },
  { 99 	, "Any private encryption scheme 	" },
  { 100 	, "GMTP 	GMTP 	" },
  { 101 	, "IFMP 	Ipsilon Flow Management Protocol 	" },
  { 102 	, "PNNI 	PNNI over IP 	" },
  { 103 	, "PIM 	Protocol Independent Multicast 	" },
  { 104 	, "ARIS 	IBM's ARIS (Aggregate Route IP Switching) Protocol 	" },
  { 105 	, "SCPS 	SCPS (Space Communications Protocol Standards) 	SCPS-TP[1]" },
  { 106 	, "QNX 	QNX 	" },
  { 107 	, "A/N 	Active Networks 	" },
  { 108 	, "IPComp 	IP Payload Compression Protocol 	RFC 3173" },
  { 109 	, "SNP 	Sitara Networks Protocol 	" },
  { 110 	, "Compaq-Peer 	Compaq Peer Protocol 	" },
  { 111 	, "IPX-in-IP 	IPX in IP 	" },
  { 112 	, "VRRP 	Virtual Router Redundancy Protocol, Common Address Redundancy Protocol (not IANA assigned) 	VRRP:RFC 3768" },
  { 113 	, "PGM 	PGM Reliable Transport Protocol 	RFC 3208" },
  { 114 	, "Any 0-hop protocol 	" },
  { 115 	, "L2TP 	Layer Two Tunneling Protocol Version 3 	RFC 3931" },
  { 116 	, "DDX 	D-II Data Exchange (DDX) 	" },
  { 117 	, "IATP 	Interactive Agent Transfer Protocol 	" },
  { 118 	, "STP 	Schedule Transfer Protocol 	" },
  { 119 	, "SRP 	SpectraLink Radio Protocol 	" },
  { 120 	, "UTI 	Universal Transport Interface Protocol 	" },
  { 121 	, "SMP 	Simple Message Protocol 	" },
  { 122 	, "SM 	Simple Multicast Protocol 	draft-perlman-simple-multicast-03" },
  { 123 	, "PTP 	Performance Transparency Protocol 	" },
  { 124 	, "IS-IS over IPv4 	Intermediate System to Intermediate System (IS-IS) Protocol over IPv4 	RFC 1142 and RFC 1195" },
  { 125 	, "FIRE 	Flexible Intra-AS Routing Environment 	" },
  { 126 	, "CRTP 	Combat Radio Transport Protocol 	" },
  { 127 	, "CRUDP 	Combat Radio User Datagram 	" },
  { 128 	, "SSCOPMCE 	Service-Specific Connection-Oriented Protocol in a Multilink and Connectionless Environment 	ITU-T Q.2111 (1999)" },
  { 129 	, "IPLT 		" },
  { 130 	, "SPS 	Secure Packet Shield 	" },
  { 131 	, "PIPE 	Private IP Encapsulation within IP 	Expired I-D draft-petri-mobileip-pipe-00.txt" },
  { 132 	, "SCTP 	Stream Control Transmission Protocol 	" },
  { 133 	, "FC 	Fibre Channel 	" },
  { 134 	, "RSVP-E2E-IGNORE 	Reservation Protocol (RSVP) End-to-End Ignore 	RFC 3175" },
  { 135 	, "Mobility Header 	Mobility Extension Header for IPv6 	RFC 6275" },
  { 136 	, "UDPLite 	Lightweight User Datagram Protocol 	RFC 3828" },
  { 137 	, "MPLS-in-IP 	Multiprotocol Label Switching Encapsulated in IP 	RFC 4023" },
  { 138 	, "MANET Protocols 	RFC 5498" },
  { 139 	, "HIP 	Host Identity Protocol 	RFC 5201" },
  { 140 	, "Shim6 	Site Multihoming by IPv6 Intermediation 	RFC 5533" },
  { 141 	, "WESP 	Wrapped Encapsulating Security Payload 	RFC 5840" },
  { 142 	, "ROHC 	Robust Header Compression" },
  { 0, 0 }
};

static char* unknown = "Unknown";

static
char* nexthdrstr
  (u_int8_t proto)
{
  unsigned i = 0;
  while (1) {
    if (ipv6protos[ i ].name == 0) {
      return unknown;
    }
    if (ipv6protos[ i ].proto == proto) {
      return ipv6protos[ i ].name;
    }
    ++i;
  }
}

void netdbg_ipv6
  (unsigned char* packet, unsigned size)
{
  unsigned char* payload;
  _16BIT payloadlength = GET_16BIT_NWO(packet, 4);
  int nextheader = packet[6];
  int hoplimit = packet[7];
  NETDBG_PRINT("-------- IPv6\n");
  if ((packet[0] >> 4) != 6) {
    NETDBG_PRINT("WARNING: Packet doesn't seem to be IPv6.\n");
  }
  if (size > payloadlength + 40) {
    NETDBG_PRINT("WARNING: Excess data.  Truncating.\n");
    size = payloadlength + 40;
  } else if (size != payloadlength + 40) {
    NETDBG_PRINT("WARNING: Packet length doesn't match given data size.\n");
  }
  NETDBG_PRINT("Paylen:  %u\n", payloadlength);
  NETDBG_PRINT("Nexthdr: %d (%s)\n", nextheader, nexthdrstr(nextheader));
  NETDBG_PRINT("Hoplim.: %d\n", hoplimit);
  NETDBG_PRINT("Source:  %x:%x:%x:%x:%x:%x:%x:%x\n",
    GET_16BIT_NWO(packet, 8),
    GET_16BIT_NWO(packet, 10),
    GET_16BIT_NWO(packet, 12),
    GET_16BIT_NWO(packet, 14),
    GET_16BIT_NWO(packet, 16),
    GET_16BIT_NWO(packet, 18),
    GET_16BIT_NWO(packet, 20),
    GET_16BIT_NWO(packet, 22)
  );
  NETDBG_PRINT("Dest:    %x:%x:%x:%x:%x:%x:%x:%x\n",
    GET_16BIT_NWO(packet, 24),
    GET_16BIT_NWO(packet, 26),
    GET_16BIT_NWO(packet, 28),
    GET_16BIT_NWO(packet, 30),
    GET_16BIT_NWO(packet, 32),
    GET_16BIT_NWO(packet, 34),
    GET_16BIT_NWO(packet, 36),
    GET_16BIT_NWO(packet, 38)
  );
  payload = packet + 40;
  size -= 40;
  while (1) {
    switch (nextheader) {
    case 0:  NETDBG_PRINT("-------- Hop By Hop\n"); goto OPTION;
    case 43: NETDBG_PRINT("-------- Routing\n"); goto OPTION;
    case 44:
      NETDBG_PRINT("-------- Fragment\n");
      NETDBG_PRINT("Fragoff: %u\n",
        ( ( payload[ 2 ] << 8) + ( payload[ 3 ] & 0xf8 ) )
      );
      NETDBG_PRINT("Morefrg: %u\n", payload[ 3 ] & 0x01);
      NETDBG_PRINT("FragID : %.8x\n", GET_32BIT_NWO(payload, 4));
      goto OPTION;
    case 50: NETDBG_PRINT("-------- ESP\n"); goto OPTION;
    case 51: NETDBG_PRINT("-------- Auth. header\n"); goto OPTION;
    case 60: NETDBG_PRINT("-------- Destination options\n"); goto OPTION;
OPTION:
      {
        unsigned nhd = payload[0];
        unsigned len = payload[1];
        unsigned siz = (len + 1) * 8;
        NETDBG_PRINT("Nexthd.: %u\n", nhd);
        NETDBG_PRINT("Noctets: %u (%u real bytes)\n", len, ((len + 1) * 8));
        if (siz <= size) {
          payload += siz;
          size -= siz;
          nextheader = nhd;
        } else {
          NETDBG_PRINT("WARNING: Exceeding given data.\n");
          goto ENDOPTIONS;
        }
      }
      continue;
    case 6:
      netdbg_ipv6_tcp(packet, payload, size);
      return;
    case 17:
      netdbg_ipv6_udp(packet, payload, size);
      return;
    case 58:
      if (size >= 8) {
        netdbg_icmpv6(packet, payload, size);
      } else {
        NETDBG_PRINT("WARNING: No further processing; remainder too small.\n");
      }
      return;
    default:
      NETDBG_PRINT("Unknown type %d: No further processing.\n", nextheader);
      return;
    }
  }
ENDOPTIONS: ;
}
