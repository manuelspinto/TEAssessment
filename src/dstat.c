#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "pstat.h"
#include "dstat.h"


void search_neighbor_deaggregation_statistics(Node *root){
	int i;
	int *ncount;
	int top = 0;

	ncount = (int*) malloc(sizeof(int)*50);

	for(i = 0; i < 50; i++)
		ncount[i] = 0;

	ChildSpread(root);

	getStat(root,&top, ncount);

	printf("#Top prefixes: %d\n",top);
	for(i= 0 ; i<50; i++)
		if(ncount[i] != 0)
			printf("%d: %d\n",i+1,ncount[i]);

	free(ncount);

	return;
}

void ChildSpread(Node *root){
  if(root->px == 1){
  	if(root->parent == root){
    	root->parent->neighbor = neighborNew(root->info.nei,NULL);
    	root->parent->nnei++;
    }
    else{
    	root->parent->child = ChildNew(root,root->parent->child);
    	if(check_neighbor(root->parent->neighbor,root->info.nei) == 0){
    		root->parent->neighbor = neighborNew(root->info.nei,root->parent->neighbor);
    		root->parent->nnei++;
    	}
    }
  }

  
  if(root->lc != NULL)
    ChildSpread(root->lc);
  if(root->rc != NULL)
    ChildSpread(root->rc);

  return;
}

void getStat(Node *root, int *top, int *ncount){

  if(root->px == 1 && root->child != NULL)
    if(root->parent == root && root->nnei != 0){
    	(*top)++;
    	ncount[root->nnei-1]++;
    	if(root->nnei == 24)
    		printf("%s: %s/%d\n",root->asn,root->info.px,root->info.mask);
    }
  
  if(root->lc != NULL)
    getStat(root->lc, top, ncount);
  if(root->rc != NULL)
    getStat(root->rc, top, ncount);

  return;
}

void printNodes(Node *root, char * str, int * index){
	int i = 0;

	(*index)++;
	if(strcmp(root->asn,"-1") != 0){
		for(i = (*index); i < 32 ; i++)
      		str[i] = '0';
    	str[32] = '\0';
    	printf("\n------>Prefix: %s/%d\n",bin2dec(str),(*index));


		printf("asn: %s\n",root->asn);
		printf("px: %d\n",root->px);
		printf("nnei: %d\n",root->nnei);
		printf("Info: \n");
		printInfo(root->info);
		printf("root->parent: \n");
		if(root->parent == NULL)
			printf("\tNULL\n");
		else
			printInfo(root->parent->info);
		printf("neighbor: \n");
		if(root->neighbor == NULL)
			printf("\tNULL\n");
		else
			printNeighbor(root->neighbor);
		printf("child: \n");
		if(root->child == NULL)
			printf("\tNULL\n");
		else
			printChild(root->child);
		printf("\n\n");
	}
  
  if(root->lc != NULL){
  	str[*index] = '0';
    printNodes(root->lc, str, index);
  }
  if(root->rc != NULL){
  	str[*index] = '1';
    printNodes(root->rc, str, index);
  }
	
  (*index)--;

}

void printChild(Child *c){
	Child *aux;

	for(aux = c; aux!= NULL ;aux = aux->next){
		printInfo(aux->node->info);
		printf("\n");
	}

}

void printInfo(nInfo i){
	printf("\tcol: %s\n",i.col);
	printf("\tnei: %s\n",i.nei);
	printf("\tori: %s\n",i.ori);
	printf("\tpx:  %s\n",i.px);
	printf("\tmask: %d\n",i.mask);
}

void printNeighbor(Neighbor *n){
	Neighbor *aux;

	for(aux=n; aux!= NULL; aux = aux->next)
		printf("\tnei: %s\n",aux->asn);
}

Neighbor * neighborNew(char *asn, Neighbor *next){
  Neighbor *new = (Neighbor *) malloc(sizeof(Neighbor));

  strcpy(new->asn, asn);
  new->next = next;
  
  return new;
}

int check_neighbor(Neighbor *n, char *asn){
	Neighbor *aux = NULL;

	for(aux = n; aux != NULL ; aux = aux->next)
		if(strcmp(aux->asn,asn) == 0)
			return 1;

	return 0;
}

int cmp_info(nInfo a, nInfo b){
	if( (strcmp(a.col,b.col) == 0) && (strcmp(a.nei,b.nei) == 0) && (strcmp(a.ori,b.ori) == 0)
		&&	(strcmp(a.px,b.px) == 0) && (a.mask == b.mask) )
		return 1;
	else
		return 0;
}


