

void search_neighbor_deaggregation_statistics(Node *root);
void search_deaggregation_length_statistics(Node *root);
void search_te_statistics(Node *root);


void ChildSpread(Node *root);
void getStat(Node *root, int *top, int *ncount, int *nprep);
void getStatLen(Node *root, int *top, int *deag, int *lencount);
void getStatTE(Node *root, int *te, int *top);
Neighbor * neighborNew(char *asn, char prep, Neighbor *next);
int check_neighbor(Neighbor *n, char *asn);
int cmp_info(nInfo a, nInfo b);

void printNodes(Node *root, char * str, int * index);
void printChild(Child *c);
void printInfo(nInfo i);
void printNeighbor(Neighbor *n);

void checkScopAndPrep(Node *root);
int checkPrep(Node *root);
int checkScoped(Node *root);


#define LEN_OFFSET 50


