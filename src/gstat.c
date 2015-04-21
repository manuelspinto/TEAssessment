#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "pstat.h"
#include "dstat.h"

#include "gstat.h"

void search_deaggregation_statistics(Node *master){
	int top = 0;
	int tot = 0;
	int i;
	int *nfreq;

	nfreq = (int*) malloc(sizeof(int)*16);

	for(i = 0; i < N_LEVEL; i++)
		nfreq[i] = 0;

	getStatD(master, &top, nfreq);

	for(i = 0; i < N_LEVEL ;i++){
		printf("<=%.2lf\t%d\t%.2lf%%\n",STEP*(i+1),nfreq[i],100*((double) ((double)nfreq[i]/ ((double) top))));
		tot += nfreq[i];
	}
	printf("\n\nTot: %d\t%.2f%%\n",top,100*((double) (((double)tot)/ ((double) top))));

}

void getStatD(Node *root, int *top, int *nfreq){

  if(root->px == 1 && root->parent == root && root->child != NULL && root->nnei > 1){
    	(*top)++;
    	nfreq[getLvl((double) ( ((double) root->nchild) / ((double) root->nnei) ) )]++;
    }
  
  if(root->lc != NULL)
    getStatD(root->lc, top, nfreq);
  if(root->rc != NULL)
    getStatD(root->rc, top, nfreq);

  return;
}

void printTopTree(Node *root){
	int top = 0;

	printTopPx(root,&top);
}

void printTopPx(Node *root, int *top){

  if((*top)/100 == 99)
  	return;

  if(root->parent == root && root->child != NULL){
  	(*top)++;
  	if((*top)%100 == 0){
	  	printf("\nFATHER:\n\t%s/%d\t%s\n", root->info.px, root->info.mask, root->info.nei);
	    printNeighbor(root->neighbor);
	    printf("CHILDREN:\n");
	    printChildren(root);
	}
  }

  if(root->lc != NULL)
    printTopPx(root->lc, top);
  if(root->rc != NULL)
    printTopPx(root->rc, top);

  return;
}

void printChildren(Node *root){
	Child *aux;
	Neighbor *nei;

	for(aux=root->child; aux != NULL ; aux = aux->next){
		printf("%d\t%s/%d\t%s(%c)\t", aux->node->info.mask - aux->node->parent->info.mask
										  , aux->node->info.px, aux->node->info.mask
										  , aux->node->info.nei, aux->node->info.prep);
		for(nei = aux->node->neighbor; nei != NULL ; nei = nei->next)
			printf("\t%s(%c)",nei->asn,nei->prep);
		printf("\n");
	}
}

int getLvl(double f){
	int i;

	/*for(i = 0; i < N_LEVEL; i++)
		if(f <= STEP*(i+1))
			return i;*/

	for(i = 0; i < N_LEVEL; i++)
		if(f <= STEP*(i+1))
			return i;
	
	return -1;
}

void masterFill(Node **root){
	int i;

	for(i = 1; i < COLL_SIZE; i++){
		mergeNeighborDEA(root[0], root[i]);
		mergeSP(root[0], root[i]);
		fprintf(stdout,":%.0lf%%\n",(double)(((double)i)/ COLL_SIZE) * 100);
	}
}

void masterFill_ipv6(Node **root){
	int i;

	for(i = 1; i < COLL_SIZE_IPV6; i++){
		/*mergeNeighborDEA_ipv6(root[0], root[i]);*/
		mergeSP_ipv6(root[0], root[i]);
		fprintf(stdout,":%.0lf%%\n",(double)(((double)i)/ COLL_SIZE_IPV6) * 100);
	}
}

void mergeNeighbor(Node *master, Node *root){
	Node *aux;

	if(root->px == 1)
		if((root->parent == root) && (root->child != NULL)){
			aux = getNode(master, root);
			if(aux != NULL && aux->px == 1)
				updateNeighbor(aux, root->neighbor);
		}

	if(root->lc != NULL)
		mergeNeighbor(master, root->lc);

	if(root->rc != NULL)
		mergeNeighbor(master, root->rc);
}

void mergeNeighborDEA(Node *master, Node *root){
	Node *aux = NULL;

	if(root->px == 1){
		aux = getNode(master, root);
		if(aux != NULL && aux->px == 1)
			updateNeighbor(aux, root->neighbor);
	}

	if(root->lc != NULL)
		mergeNeighborDEA(master, root->lc);

	if(root->rc != NULL)
		mergeNeighborDEA(master, root->rc);
}

void mergeNeighborDEA_ipv6(Node *master, Node *root){
	Node *aux = NULL;

	if(root->px == 1){
		aux = getNode_ipv6(master, root);
		if(aux != NULL && aux->px == 1)
			updateNeighbor(aux, root->neighbor);
	}

	if(root->lc != NULL)
		mergeNeighborDEA_ipv6(master, root->lc);

	if(root->rc != NULL)
		mergeNeighborDEA_ipv6(master, root->rc);
}

void mergeSP(Node *master, Node *root){
	Node *aux;

	if(root->px == 1){
		aux = getNode(master, root);
		if(aux != NULL && aux->px == 1) {
			if((root->parent == root) && (root->child != NULL))
				updateSP(aux, root);
			else 
				updateP(aux, root);
		}
	}

	if(root->lc != NULL)
		mergeSP(master, root->lc);

	if(root->rc != NULL)
		mergeSP(master, root->rc);
}

void mergeSP_ipv6(Node *master, Node *root){
	Node *aux;

	if(root->px == 1){
		aux = getNode_ipv6(master, root);
		if(aux != NULL && aux->px == 1) {
			if((root->parent == root) && (root->child != NULL))
				updateSP(aux, root);
			else 
				updateP(aux, root);
		}
	}

	if(root->lc != NULL)
		mergeSP_ipv6(master, root->lc);

	if(root->rc != NULL)
		mergeSP_ipv6(master, root->rc);
}

void updateP(Node *master, Node *root){
	if(master->info.prep == 'N' && root->info.prep == 'P')
		master->info.prep = 'P';
}

void updateSP(Node *master, Node *root){
	if(master->GPrep != 1 && root->GPrep == 1)
		master->GPrep = 1;
	if(master->GScop != 1 && root->GScop == 1)
		master->GScop = 1;
}

void updateNeighbor(Node *master, Neighbor *nei){
	Neighbor *naux;
	Neighbor *maux;
	int found;

	if(master == NULL)
		return;

	for(naux = nei; naux != NULL; naux = naux->next){
		found = 0;
		for(maux = master->neighbor; maux != NULL; maux = maux->next){
			if(strcmp(naux->asn, maux->asn) == 0 && naux->prep == maux->prep){
				found = 1;
				break;
			}
		}
		if(found == 0){
			master->neighbor = neighborNew(naux->asn, naux->prep, master->neighbor);
			(master->nnei)++;
		}
	}
}

Node *getNode(Node *root, Node *node){
	int i;
	Node *p = root;
	int mask = node->info.mask;
	char ip[129];

	strcpy(ip, dec2bin(node->info.px));

	for(i= 0; i<mask;i++){
		if(ip[i] == '0'){
			if(p->lc != NULL)
				p = p->lc;
			else
				return NULL;
		}else{
			if(p->rc != NULL)
				p = p->rc;
			else
				return NULL;
		} 
	}
			
	return p; 
}

Node *getNode_ipv6(Node *root, Node *node){
	int i;
	Node *p = root;
	int mask = node->info.mask;
	char ip[129];

	strcpy(ip, hex2binv6(node->info.px));

	for(i= 0; i<mask;i++){
		if(ip[i] == '0'){
			if(p->lc != NULL)
				p = p->lc;
			else
				return NULL;
		}else{
			if(p->rc != NULL)
				p = p->rc;
			else
				return NULL;
		} 
	}
			
	return p; 
}


