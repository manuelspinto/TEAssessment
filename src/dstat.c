#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "pstat.h"
#include "dstat.h"


void search_neighbor_deaggregation_statistics(Node *root){
	int i;
	int *ncount;
  int *nprep;
	int top = 0;

	ncount = (int*) malloc(sizeof(int)*50);
  nprep = (int*) malloc(sizeof(int)*50);

	for(i = 0; i < 50; i++){
		ncount[i] = 0;
    nprep[i] = 0;
  }

	getStat(root,&top, ncount, nprep);

	printf("#Top prefixes: %d\n\n",top);
	printf("#nNei\t#nTop\t%%nTop\t%%Prep\t%%PrepT\n");
	for(i= 0 ; i<50; i++)
		if(ncount[i] != 0)
			printf("%d\t%d\t%.2lf%%\t%.2lf%%\t%.2lf%%\n",i+1,ncount[i],(((double)ncount[i])/((double)top)) * 100,
                                                            (((double)nprep[i])/((double)top)) * 100,
                                                            (((double)nprep[i])/((double)ncount[i])) * 100);

	free(ncount);

	return;
}

void search_te_statistics(Node *root){
  int i;
  int *te;
  int *dea;
  int top =0;
  int tetot = 0;

  te = (int*) malloc(3*sizeof(int));
  dea = (int*) malloc(2*sizeof(int));

  for(i = 0; i < 3; i++)
    te[i] = 0;

  for(i = 0; i<3; i++)
    dea[i] = 0;

  getStatTE(root, te, dea, &top);

  printf("\ntop = %d\n\n", top);
  printf("Scoped?\tPrep?\t#top\t%%top\n");
  printf("NO\tNO\t%d\t%.2lf%%\t%d\n"  , te[0], (((double)te[0])/((double)top)) * 100, dea[0]);
  printf("YES\tNO\t%d\t%.2lf%%\t%d\n" , te[1], (((double)te[1])/((double)top)) * 100, dea[1]);
  printf("-\tYES\t%d\t%.2lf%%\t%d\n", te[2], (((double)te[2])/((double)top)) * 100, dea[2]);

  for(i = 1; i<3; i++)
    tetot+=te[i];



  printf("\nTE:\t%d\t%.2lf%%\n", tetot, (((double)tetot)/((double)top)) * 100);
  printf("NO-TE\t%d\t%.2lf%%\n", te[0], (((double)te[0])/((double)top)) * 100);

}

void search_deaggregation_length_statistics(Node *root){
  int i;
  int *lencount;
  int top = 0;
  int deag = 0;
  int worse = 0;
  int better = 0;

  lencount = (int*) malloc(sizeof(int)*(2*LEN_OFFSET+1));

  for(i = 0; i < (2*LEN_OFFSET+1); i++)
    lencount[i] = 0;

  getStatLen(root,&top,&deag, lencount);

  printf("\n** Path length statistics **\n\n");
  printf("#Top prefixes: %d\t#Deag prefixes: %d\n\n",top,deag);
  printf("#nhP-nhC\t#nDeag\t%%nDeag\n");
  for(i= 0 ; i< (2*LEN_OFFSET+1); i++)
    if(lencount[i] != 0)
      printf("%d\t\t%d\t(%.2lf%%)\n",-LEN_OFFSET + i,lencount[i],(((double)lencount[i])/((double)deag)) * 100);
  printf("\n\n");

  for(i = 0 ; i < LEN_OFFSET; i++)
    if(lencount[i] != 0)
      worse +=lencount[i];

  for(i = LEN_OFFSET + 1 ; i < (2*LEN_OFFSET+1); i++)
    if(lencount[i] != 0)
      better +=lencount[i];
  
  printf("#worse: %d (%.2lf%%)\n",worse,(((double)worse)/((double)deag)) * 100);
  printf("#equal: %d (%.2lf%%)\n",lencount[LEN_OFFSET],(((double)lencount[LEN_OFFSET])/((double)deag)) * 100);
  printf("#better: %d (%.2lf%%)\n",better,(((double)better)/((double)deag)) * 100);

  free(lencount);

  return;
}

void ChildSpread(Node *root){
  if(root->px == 1){
  	if(root->parent == root){
    	root->parent->neighbor = neighborNew(root->info.nei,root->info.prep, root->neighbor);
    	root->parent->nnei++;
    }
    else{
    	root->parent->child = ChildNew(root,root->parent->child);
      root->parent->nchild++;

      root->neighbor = neighborNew(root->info.nei, root->info.prep, root->neighbor); /*****/
    	
      /* COMMENT BLOCK IF PRINTING IS USED */
      /*if(check_neighbor(root->parent->neighbor,root->info.nei) == 0){
    		root->parent->neighbor = neighborNew(root->info.nei,root->info.prep,root->parent->neighbor);
    		root->parent->nnei++;
    	}*/
    }
  }

  
  if(root->lc != NULL)
    ChildSpread(root->lc);
  if(root->rc != NULL)
    ChildSpread(root->rc);

  return;
}

void checkScopAndPrep(Node *root){
  if(root->parent == root && (root->child != NULL)){
      root->GPrep = checkPrep(root);
      root->GScop = checkScoped(root);
  }

  if(root->lc != NULL)
    checkScopAndPrep(root->lc);
  if(root->rc != NULL)
    checkScopAndPrep(root->rc);

  return;
}

int checkPrep(Node *root){
  Child *aux;

  if(root->info.prep == 'P')
    return 1;
  for(aux=root->child; aux != NULL; aux = aux->next)
    if(aux->node->info.prep == 'P')
      return 1;
  
  return 0;
}

int checkScoped(Node *root){
  if(root->nnei > 1)
    return 1;
  return 0;
}

void getStat(Node *root, int *top, int *ncount, int *nprep){

  if(root->px == 1 && root->parent == root && root->child != NULL){
    	(*top)++;
    	ncount[root->nnei-1]++;
      if(root->info.prep == 'P')
        nprep[root->nnei-1]++;
  }
  
  if(root->lc != NULL)
    getStat(root->lc, top, ncount, nprep);
  if(root->rc != NULL)
    getStat(root->rc, top, ncount, nprep);

  return;
}
void getStatTE(Node *root, int *te, int *dea, int *top){
  if(root->px == 1 && root->parent == root && root->child != NULL){ /** top **/
    (*top)++;
    if(root->GScop == 0 && root->GPrep == 0){
      te[0]++;
      dea[0]+=root->nchild;
    }
    else {
      if(root->GScop == 1 && root->GPrep == 0){
        te[1]++;
        dea[1]+=root->nchild;
      }
      else {
        te[2]++;
        dea[2]+=root->nchild;
      }
    }
  }
  
  if(root->lc != NULL)
    getStatTE(root->lc, te, dea, top);
  if(root->rc != NULL)
    getStatTE(root->rc, te, dea, top);

  return;
}



void getStatLen(Node *root, int *top, int *deag, int *lencount){
  Child *aux;

  if(root->px == 1 && root->parent == root && root->child != NULL){
      (*top)++;
      for(aux = root->child; aux != NULL; aux = aux->next){
        lencount[LEN_OFFSET + (root->info.hlen - aux->node->info.hlen)]++;
        (*deag)++;
      }
  }
  
  if(root->lc != NULL)
    getStatLen(root->lc, top, deag, lencount);
  if(root->rc != NULL)
    getStatLen(root->rc, top, deag, lencount);

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
  printf("\tprep: %c\n",i.prep);
  printf("\tplen: %d\n\n",i.plen);
}

void printNeighbor(Neighbor *n){
	Neighbor *aux;

	for(aux=n; aux!= NULL; aux = aux->next)
		printf("\tnei: %s(%c)\n",aux->asn, aux->prep);
}

Neighbor * neighborNew(char *asn, char prep,  Neighbor *next){
  Neighbor *new = (Neighbor *) malloc(sizeof(Neighbor));

  strcpy(new->asn, asn);
  new->prep = prep;
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



