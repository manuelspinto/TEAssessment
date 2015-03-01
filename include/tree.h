#include <stdio.h>
#include <stdlib.h>

typedef struct neighbor{
	char asn[AS_SIZE];
	struct neighbor *next;
} Neighbor;

typedef struct line{
  char ip[33];
  char asn[AS_SIZE];
  int mask;
} Line;

typedef struct node_info{
	char col[AS_SIZE];
	char nei[AS_SIZE];
	char ori[AS_SIZE];
	char px[33];
	int mask;
} nInfo;

typedef struct child{
	struct node *node;
	struct child *next;
} Child;

typedef struct node{
  char asn[128];
  int px;
  int nnei;
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
void TreeInsert(Node * root, Line l, nInfo info);
void TreeSpread(Node * root);
void TreeParentSpread(Node * root);
void TablePrint(Node * root, Node * p, char * str, int * index, int *totpx, int *delpx, int *deapx, int *lonpx, int *toppx);
void TreeClean(Node * root);

Node *BuildPrefixTree(FILE *fp);
void get_ori_px_mask(char *buff, char *as_ori, char *px, char *mask);

