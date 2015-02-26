#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"

Node * NodeNew(){
  Node *new = (Node *) malloc(sizeof(Node));
  strcpy(new->asn, "-1");
  new->px = 0;
  new->lc = NULL;
 	new->rc = NULL;
  return new;
}

int asncmp(char *asn, char *pasn){
  char buff[128];
  char buff2[128];
  int i, j;
  int i2,j2;
  int len;
  int len2;

  i = 0;
  j = 0;

  len = strlen(asn);
  len2 = strlen(pasn);

  for(i = 0; i < len ; i++, j++){
    if(asn[i] == '_'){
      buff[j] = '\0';

      j2 = 0;
      for(i2 = 0; i2 < len ; i2++, j2++){
        if(pasn[i2] == '_'){
          buff2[j2] = '\0';
          if(strcmp(buff,buff2) == 0){
            return 0;
          }

          j2 = -1;
        }
        else if(pasn[i2] == ','){
          break;
        }else
          buff2[j2] = pasn[i2];

      }
      buff2[j2] = '\0';
      if(strcmp(buff,buff2) == 0){
        return 0;
      }


      j = -1;
    }
    else if(asn[i] == ','){
      break;
    }else
      buff[j] = asn[i];

  }
  buff[j] = '\0';

  
  j2 = 0;
  for(i2 = 0; i2 < len2 ; i2++, j2++){
    if(pasn[i2] == '_'){
      buff2[j2] = '\0';
      if(strcmp(buff,buff2) == 0){
        return 0;
      }

      j2 = -1;
    }else if(pasn[i2] == ','){
      break;
    }else
      buff2[j2] = pasn[i2];
  }
  buff2[j2] = '\0';
  if(strcmp(buff,buff2) == 0){
    return 0;
  }

  return 1;
}

char * dec2bin(char *decstr){
  int dec[4];
  char *bin;
  int i, j, npow;
  int twopow[8] = {1,2,4,8,16,32,64,128};

  bin = (char *) malloc(sizeof(char) * 33);
  if(bin == NULL){
    fprintf(stderr,"Error allocating memory for 'bin'\n");
    exit(1);
  }

  sscanf(decstr,"%d.%d.%d.%d",&dec[0],&dec[1],&dec[2],&dec[3]);

  i = 0;
  for(j = 0; j < 4; j++){
    for(npow = 7; npow >= 0; npow--, i++){
      if(dec[j]/twopow[npow] == 1){
        dec[j] = dec[j]% twopow[npow];
        bin[i] = '1';
      } 
      else
        bin[i] = '0';
    }
  }
  bin[32] = '\0';

  return bin;
}

char * bin2dec(char *bin){
  int npow, i, j;
  char *decstr;
  int twopow[8] = {1,2,4,8,16,32,64,128};
  int dec[4] = {0,0,0,0};

  decstr = (char*) malloc(sizeof(char) * 16);
  if(decstr == NULL){
    fprintf(stderr,"Error allocating memory for 'decstr'\n");
    exit(1);
  }

  i = 0;
  for(j = 0; j < 4; j++)
    for(npow = 7; npow >= 0; npow--, i++)
      if(bin[i] == '1')
        dec[j] += twopow[npow];

  sprintf(decstr,"%d.%d.%d.%d",dec[0],dec[1],dec[2],dec[3]);

  return decstr;
} 

void TreeInsert(Node * root, Line l){
  int i;
  Node *p = root;
  int mask = l.mask;
  
  for(i = 0; i < mask; i++){
    if(l.ip[i] == '0'){
      if(p->lc == NULL){
        p->lc = NodeNew();
      }
      p = p->lc;
    }
    else{
      if(p->rc == NULL){
        p->rc = NodeNew();
      }
      p = p->rc;
    }
  }
  strcpy(p->asn, l.asn);
  p->px = 1;
  return;
}

void TreeSpread(Node * root, Node * p){
  
  
  /* Conjunto de operações responsáveis por propagar na àrvore */
  if(root == NULL) return;
  if(root->lc != NULL && (strcmp(root->lc->asn, "-1") == 0))
    strcpy(root->lc->asn, root->asn);
  if(root->rc != NULL && (strcmp(root->rc->asn, "-1") == 0))
    strcpy(root->rc->asn, root->asn);
  
  TreeSpread(root->lc, root);
  TreeSpread(root->rc, root);
  
  return;
}


void TablePrint(Node * root, Node * p, char * str, int * index, int *totpx, int *delpx, int *deapx, int *lonpx, int *toppx){
  int i = 0;

  (*index)++;
  if(root->px == 1){
    for(i = (*index); i < 32 ; i++)
      str[i] = '0';
    str[32] = '\0';

    if(strcmp(p->asn,"-1") == 0) {
      if(root->lc != NULL || root->rc != NULL)
        (*toppx)++;
      else
        (*lonpx)++;
    }else{
      if(asncmp(p->asn,root->asn) == 0)
        (*deapx)++;
      else
        (*delpx)++; 
    }

  }
  
  if(root->lc != NULL){
    str[*index] = '0';
    TablePrint(root->lc, root, str, index, totpx, delpx, deapx, lonpx, toppx);
  }

  if(root->rc != NULL){
    str[*index] = '1';
    TablePrint(root->rc, root, str, index, totpx, delpx, deapx, lonpx, toppx);
  }
  (*index)--;
  
  return;
}

void TreeClean(Node * root){
  
  if (root == NULL)
    return;
  TreeClean(root->lc);
  TreeClean(root->rc);
  
  free(root);
}

void TreeBuild(Node *root, char *fname){
  FILE *fp;
  char buff[512];
  char ip[16];
  char asn[128];
  int mask;
  Line line;
  
  fp = fopen(fname,"r");
  if(fp == NULL){
    printf("ERROR: Impossivel abrir o ficheiro '%s'\n",fname);
    exit(1);
  }
  
  while (fgets(buff, sizeof(buff), fp) != NULL ){
    sscanf(buff,"%s\t%d\t%s",ip,&mask,asn);
    strcpy(line.ip, dec2bin(ip));
    line.mask = mask;
    strcpy(line.asn, asn);

    TreeInsert(root,line);
  }
  fclose(fp);
  return;
}

