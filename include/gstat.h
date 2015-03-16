
void masterFill(Node **root);
void mergeNeighbor(Node *master, Node *root);
void updateNeighbor(Node *master, Neighbor *nei);
Node *getNode(Node *root, Node *node);

void search_deaggregation_statistics(Node *master);
void getStatD(Node *root, int *top, int *nfreq);

int getLvl(double f);

#define STEP 0.5
#define N_LEVEL 100
