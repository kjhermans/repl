/**
 * Utility to bridge two network interfaces. Featuring:
 * - Diode function.
 * - Dump on stderr function.
 * - Exclude IP from being dumped.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/select.h>

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#include "netdbg/netdbg.h"

extern int queryargs
  (
    unsigned argc,
    char* argv[],
    char option,
    char* longoption,
    unsigned index,
    int expectvalue,
    char** remainder,
    char** value
  );

static
int rawsocket_open(char* device, unsigned mtu, int protocol)
{
  int rawsock;
  struct sockaddr_ll sll;
  struct ifreq ifr;

  if((rawsock = socket(PF_PACKET, SOCK_RAW, htons(protocol)))== -1)
  {
    fprintf(stderr, "Could not create socket for device '%s'\n", device);
    return -1;
  }

  memset(&sll, 0, sizeof(sll));
  memset(&ifr, 0, sizeof(ifr));
    
  strncpy((char *)ifr.ifr_name, device, IFNAMSIZ - 1);
  if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1)
  {
    fprintf(stderr, "Device index of '%s' not found\n", device);
    return -1;
  }

  sll.sll_family = AF_PACKET;
  sll.sll_ifindex = ifr.ifr_ifindex;
  sll.sll_protocol = htons(protocol); 

  if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
  {
    fprintf(stderr, "Could not bind socket to device '%s'\n", device);
    return -1;
  }

  if (mtu) {
    if (mtu > 1500) {
      mtu += 32;
    }
    ifr.ifr_mtu = mtu; //your MTU size here
    if (ioctl(rawsock, SIOCSIFMTU, (caddr_t)&ifr) < 0) {
      fprintf(stderr, "Could not set MTU %u to device '%s'\n", mtu, device);
      return -1;
    }
  }

  fprintf(stderr, "Setting PACKET_AUXDATA on socket.\n");
  int option = 1;
  if (setsockopt(rawsock, SOL_PACKET, PACKET_AUXDATA, &option, sizeof(option))
       == -1)
  {
    fprintf(stderr, "Setting PACKET_AUXDATA on socket failed.\n");
  }

  return rawsock;
}

static
int dump = 0;

static
int pass
  (int fdin, int fdout)
{
  unsigned char buf[ 4096 ];

  ssize_t r = read(fdin, buf, sizeof(buf));
  if (r == -1) {
    return -1;
  }
  if (dump) {
    netdbg_ethernet(buf, r);
  }
  write(fdout, buf, r);
  return 0;
}

int main
  (int argc, char* argv[])
{
  int fd0, fd1;
  char* ifx0, * ifx1;
  int diode = 0;

  if (queryargs(argc, argv, 'i', "interface", 0, 1, 0, &ifx0)) {
    fprintf(stderr, "No interface argument given.\n");
    exit(1);
  }
  if (queryargs(argc, argv, 'i', "interface", 1, 1, 0, &ifx1)) {
    fprintf(stderr, "No second interface argument given.\n");
    exit(2);
  }
  if (queryargs(argc, argv, 'd', "diode", 0, 0, 0, 0) == 0) {
    fprintf(stderr, "Diode function %s -> %s\n", ifx0, ifx1);
    diode = 1;
  }
  if (queryargs(argc, argv, 'D', "dump", 0, 0, 0, 0) == 0) {
    dump = 1;
  }
  if ((fd0 = rawsocket_open(ifx0, 0, ETH_P_ALL)) == -1) {
    fprintf(stderr, "Could not open interface '%s'.\n", ifx0);
    exit(3);
  }
  if ((fd1 = rawsocket_open(ifx1, 0, ETH_P_ALL)) == -1) {
    fprintf(stderr, "Could not open interface '%s'.\n", ifx1);
    exit(4);
  }

  while (1) {
    fd_set readset;

    FD_ZERO(&readset);
    FD_SET(fd0, &readset);
    FD_SET(fd1, &readset);
    switch (select(fd1+1, &readset, 0, 0, 0)) {
    case -1:
      fprintf(stderr, "Select error.\n");
      exit(5);
    default:
      if (FD_ISSET(fd0, &readset)) {
        pass(fd0, fd1);
      }
      if (FD_ISSET(fd1, &readset)) {
        if (!diode) {
          pass(fd1, fd0);
        }
      }
    }
  }
}
