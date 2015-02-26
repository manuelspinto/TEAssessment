#include <stdio.h>
#include <stdlib.h>

/*
 *	Estrutura que define os parametros de um nó na àrvore binária
 */
typedef struct node{
  char asn[128];
  int px;
  struct node *lc; /* ponteiro para filho esquerdo */
  struct node *rc; /* ponteiro para filho direito */
} Node;


typedef struct line{
  char ip[33];
  char asn[128];
  int mask;
} Line;

Node * NodeNew();
int asncmp(char *asn, char *pasn);
char * dec2bin(char *decstr);
char * bin2dec(char *bin);
void TreeInsert(Node * root, Line l);
void TreeSpread(Node * root, Node * p);
void TablePrint(Node * root, Node * p, char * str, int * index, int *totpx, int *delpx, int *deapx, int *lonpx, int *toppx);
void TreeClean(Node * root);
void TreeBuild(Node *root, char *fname);

