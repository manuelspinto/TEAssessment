/* global.h */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#define BUFF_SIZE 512
#define COLL_SIZE 34
#define COLL_SIZE_IPV6 7
#define AS_SIZE 40

extern char collector_asn[COLL_SIZE][10];
extern char collector_asn_ipv6[COLL_SIZE_IPV6][10];

extern char collector_px[COLL_SIZE][16];

extern char ip_version;

