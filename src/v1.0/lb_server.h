#ifndef MME_SERVER_H
#define MME_SERVER_H

#include "lb.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "sync.h"
#include "udp_client.h"
#include "utils.h"

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/gauge_builder.h>


extern MmeLB g_lbmme;

void check_usage(int);
void init(char**);
void run();
int handle_ue(int, int);

string resolve_host2(string host) {
  hostent * record = gethostbyname(host.c_str());
  if (record == NULL) {
    printf("%s is unknown\n", host.c_str());
    return "";
  }
  in_addr * address = (in_addr * )record->h_addr;
  return inet_ntoa(* address);
}


#endif /* MME_SERVER_H */

