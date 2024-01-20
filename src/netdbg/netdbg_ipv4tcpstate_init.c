#include "netdbg.h"

struct ipv4tcpstate* netdbg_ipv4tcpstate_init
  (
    struct ipv4tcpstate* state,
    _32BIT fromhost,
    _32BIT tohost,
    _16BIT fromport,
    _16BIT toport
  )
{
  if (state) {
    memset(state, 0, sizeof(struct ipv4tcpstate));
    state->fromhost = fromhost;
    state->tohost = tohost;
    state->fromport = fromport;
    state->toport = toport;
  }
  return state;
}
