
void masterFill(Node **root);
void mergeNeighbor(Node *master, Node *root);
void mergeSP(Node *master, Node *root);
void updateNeighbor(Node *master, Neighbor *nei);
void mergeNeighborDEA(Node *master, Node *root);
Node *getNode(Node *root, Node *node);
void updateSP(Node *master, Node *root);
void updateP(Node *master, Node *root);

void search_deaggregation_statistics(Node *master);
void getStatD(Node *root, int *top, int *nfreq);
void printTopPx(Node *root, int *top);
void printTopTree(Node *root);
void printChildren(Node *root);

int getLvl(double f);

#define STEP 0.5
#define N_LEVEL 10
