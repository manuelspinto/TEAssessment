

void search_neighbor_deaggregation_statistics(Node *root);


void ChildSpread(Node *root);
void getStat(Node *root, int *top, int *ncount);
Neighbor * neighborNew(char *asn, Neighbor *next);
int check_neighbor(Neighbor *n, char *asn);
int cmp_info(nInfo a, nInfo b);

void printNodes(Node *root, char * str, int * index);
void printChild(Child *c);
void printInfo(nInfo i);
void printNeighbor(Neighbor *n);



