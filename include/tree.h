#include <stdio.h>
#include <stdlib.h>

typedef struct neighbor{
	char asn[AS_SIZE];
  char prep;
	struct neighbor *next;
} Neighbor;

typedef struct line{
  char ip[129];
  char asn[AS_SIZE];
  int mask;
} Line;

typedef struct node_info{
	char col[AS_SIZE];
	char nei[AS_SIZE];
	char ori[AS_SIZE];
	char px[33];
	int mask;
	char prep;
  	int plen; /* path length */
  	int hlen; /* hop length */
} nInfo;

typedef struct child{
	struct node *node;
	struct child *next;
} Child;

typedef struct node{
  char asn[128];
  int px;
  int nnei; /* Number of neighbhors */
  int nchild;
  int nneiP;

  int GPrep;
  int GScop;


  struct node *parent;
  struct neighbor *neighbor;
  struct node_info info;
  struct node *lc; /* ponteiro para filho esquerdo */
  struct node *rc; /* ponteiro para filho direito */
  struct child *child;
} Node;

Node * NodeNew();
Child * ChildNew(Node *node, Child *next);
int asncmp(char *asn, char *pasn);
char * dec2bin(char *decstr);
char * bin2dec(char *bin);
char *hex2binv6(char *px);
void insert_block(char *px, char block[4], int b);
char * hex2bin4(char hex);

void TreeInsert(Node * root, Line l, nInfo info);
void TreeSpread(Node * root);
void TreeParentSpread(Node * root);
void TablePrint(Node * root, Node * p, char * str, int * index, int *totpx, int *delpx, int *deltpx, int *deapx, int *lonpx, int *toppx, int*topdea, int *prepx, int *preptype);
void TablePrint_ipv6(Node * root, Node * p, char * str, int * index, int *totpx, int *delpx, int *deltpx, int *deapx, int *lonpx, int *toppx, int*topdea, int *prepx, int *preptype);
void TreeClean(Node * root);
void printTree(Node *root);
void TopTreePrint(Node * root, Node * p, int *ret, int *topcnt);


Node *BuildPrefixTree(FILE *fp);
Node *BuildPrefixTree_ipv6(FILE *fp);
void get_ori_px_mask(char *buff, char *as_ori, char *px, char *mask);

