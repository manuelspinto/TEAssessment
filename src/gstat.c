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

	for(i = 1; i < 36; i++){
		mergeNeighbor(root[0], root[i]);
		fprintf(stdout,":%.0lf%%\n",(double)(((double)i)/ 36) * 100);
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

void updateNeighbor(Node *master, Neighbor *nei){
	Neighbor *naux;
	Neighbor *maux;
	int found;

	if(master == NULL)
		return;

	for(naux = nei; naux != NULL; naux = naux->next){
		found = 0;
		for(maux = master->neighbor; maux != NULL; maux = maux->next){
			if(strcmp(naux->asn, maux->asn) == 0){
				found = 1;
				break;
			}
		}
		if(found == 0){

			master->neighbor = neighborNew(naux->asn,master->neighbor);
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


