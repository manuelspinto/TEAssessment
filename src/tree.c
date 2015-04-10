#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "dstat.h"

Node * NodeNew(){
  Node *new = (Node *) malloc(sizeof(Node));
  strcpy(new->asn, "-1");

  new->parent = NULL;

  new->px = 0;
  new->neighbor = NULL;
  new->lc = NULL;
 	new->rc = NULL;
  new->child = NULL;
  new->nnei = 0;
  new->nneiP = 0;
  new->nchild = 0;
  new->GPrep = 0;
  new->GScop = 0;

  return new;
}

Child * ChildNew(Node *node, Child *next){
  Child *new = (Child *) malloc(sizeof(Child));

  new->node = node;
  new->next = next;
  
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

void TreeInsert(Node * root, Line l, nInfo info){
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
  p->info = info;

  return;
}

void TreeSpread(Node * root){
  
  if(root == NULL) return;
  if(root->lc != NULL && (strcmp(root->lc->asn, "-1") == 0))
    strcpy(root->lc->asn, root->asn);
  if(root->rc != NULL && (strcmp(root->rc->asn, "-1") == 0))
    strcpy(root->rc->asn, root->asn);
  
  TreeSpread(root->lc);
  TreeSpread(root->rc);
  
  return;
}

Node *BuildPrefixTree(FILE *fp){
  char buff[BUFF_SIZE];
  char px[33], mask[10];
  char as_ori[AS_SIZE];
  char as_nei[AS_SIZE];
  char as_col[AS_SIZE];
  char prep;
  int  plen;
  int  hlen;

  Line line;
  nInfo n_info;
  Node *root = NodeNew();
  root->parent = root;

  while(fgets(buff, sizeof(buff), fp) != NULL){
    if(buff[strlen(buff)-1] == '\n')
      buff[strlen(buff)-1] = '\0';

    
    sscanf(buff,"%c %d %d %s %s %s %s %s",&prep, &plen, &hlen, as_col, as_nei, as_ori, px, mask);

    n_info.prep = prep;
    n_info.plen = plen;
    n_info.hlen = hlen;
    strcpy(n_info.col, as_col);
    strcpy(n_info.nei, as_nei);
    strcpy(n_info.ori, as_ori);
    strcpy(n_info.px , px);
    n_info.mask = atoi(mask);

    strcpy(line.ip, dec2bin(px));
    line.mask = atoi(mask);
    strcpy(line.asn, as_ori);

    TreeInsert(root,line,n_info);
  }

  TreeSpread(root);

  return root;
}

Node *BuildPrefixTree_ipv6(FILE *fp){
  char buff[BUFF_SIZE];
  char px[33], mask[10];
  char as_ori[AS_SIZE];
  char as_nei[AS_SIZE];
  char as_col[AS_SIZE];
  char prep;
  int  plen;
  int  hlen;

  Line line;
  nInfo n_info;
  Node *root = NodeNew();
  root->parent = root;

  while(fgets(buff, sizeof(buff), fp) != NULL){
    if(buff[strlen(buff)-1] == '\n')
      buff[strlen(buff)-1] = '\0';

    
    sscanf(buff,"%c %d %d %s %s %s %s %s",&prep, &plen, &hlen, as_col, as_nei, as_ori, px, mask);

    n_info.prep = prep;
    n_info.plen = plen;
    n_info.hlen = hlen;
    strcpy(n_info.col, as_col);
    strcpy(n_info.nei, as_nei);
    strcpy(n_info.ori, as_ori);
    strcpy(n_info.px , px);
    n_info.mask = atoi(mask);

    strcpy(line.ip, hex2binv6(px));
    line.mask = atoi(mask);
    strcpy(line.asn, as_ori);

    TreeInsert(root,line,n_info);
  }

  TreeSpread(root);

  return root;
}


void TreeParentSpread(Node * root){
  
  if(root == NULL) return;
  if(root->lc != NULL && strcmp(root->lc->asn,"-1") != 0){
    if(strcmp(root->lc->asn, root->asn) == 0)
      root->lc->parent = root->parent;
    else
      root->lc->parent = root->lc;
  }
  if(root->rc != NULL && strcmp(root->rc->asn,"-1") != 0){
    if(strcmp(root->rc->asn, root->asn) == 0)
      root->rc->parent = root->parent;
    else
      root->rc->parent = root->rc;
  }
  
  TreeParentSpread(root->lc);
  TreeParentSpread(root->rc);
  
  return;
}


void TopTreePrint(Node * root, Node * p, int *ret, int *topcnt){
  int cnt = 0;

  if(root->px == 1){

    if(strcmp(p->asn,"-1") == 0) {
      if(root->lc != NULL || root->rc != NULL){
        /*(*toppx)++;*/
        (*topcnt)++;
        if((*topcnt) % 100 == 0){

          printf("\nFATHER:\n\t(%s)\t%s/%d\t%s\t%c\n", root->asn, root->info.px, root->info.mask, root->info.nei, root->info.prep);
          printf("CHILDREN:\n");
          if((*topcnt)%100 == 0 && (*topcnt)/100 == 99)
            cnt = 1;
        }
      }
      else
        /*(*lonpx)++;*/
        return;
    }else{
      if((*topcnt)%100 == 0 && (*topcnt)/100 <100)
        printf("%d\t(%s)\t%s/%d\t%s\t%c\n",root->info.mask - root->parent->info.mask, root->asn, root->info.px, root->info.mask, root->info.nei, root->info.prep);
    }
  }


  if((*ret) == 1)
    return;
  
  if(root->lc != NULL)
    TopTreePrint(root->lc, root, ret, topcnt);
  

  if((*ret) == 1)
    return;

  if(root->rc != NULL)
    TopTreePrint(root->rc, root, ret, topcnt);
  
  
  if(cnt == 1)
    (*ret) =  1;

  return;
}


void TablePrint(Node * root, Node * p, char * str, int * index, int *totpx
              , int *delpx, int *deltpx, int *deapx, int *lonpx, int *toppx, int *topdea
              , int *prepx, int *preptype){
  int i = 0;

  (*index)++;
  if(root->px == 1){
    for(i = (*index); i < 32 ; i++)
      str[i] = '0';
    str[32] = '\0';

    if(root->info.prep == 'P')
      (*prepx)++;

    if(strcmp(p->asn,"-1") == 0) {
      if(root->lc != NULL || root->rc != NULL){
        if(root->child != NULL){
          (*topdea)++;
          if(root->info.prep == 'P') preptype[5]++;
        }else{
          (*toppx)++;
          if(root->info.prep == 'P') preptype[0]++;
        }
      }
      else{
        (*lonpx)++;
        if(root->info.prep == 'P') preptype[1]++;
      }
    }else{
      if(asncmp(p->asn,root->asn) == 0){
        (*deapx)++;
        if(root->info.prep == 'P') preptype[2]++;
      }
      else{
        if(root->child != NULL){
          (*deltpx)++;
          if(root->info.prep == 'P') preptype[3]++;
        }else{
          (*delpx)++; 
          if(root->info.prep == 'P') preptype[4]++;
        }
      }
    }
  }
  
  if(root->lc != NULL){
    str[*index] = '0';
    TablePrint(root->lc, root, str, index, totpx, delpx, deltpx, deapx, lonpx, toppx, topdea, prepx, preptype);
  }

  if(root->rc != NULL){
    str[*index] = '1';
    TablePrint(root->rc, root, str, index, totpx, delpx, deltpx, deapx, lonpx, toppx, topdea, prepx, preptype);
  }
  (*index)--;
  
  return;
}

void TablePrint_ipv6(Node * root, Node * p, char * str, int * index, int *totpx, int *delpx, int *deapx, int *lonpx, int *toppx, int *prepx){
  int i = 0;

  (*index)++;
  if(root->px == 1){
    for(i = (*index); i < 128 ; i++)
      str[i] = '0';
    str[128] = '\0';

    if(root->info.prep == 'P')
      (*prepx)++;

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
    TablePrint_ipv6(root->lc, root, str, index, totpx, delpx, deapx, lonpx, toppx, prepx);
  }

  if(root->rc != NULL){
    str[*index] = '1';
    TablePrint_ipv6(root->rc, root, str, index, totpx, delpx, deapx, lonpx, toppx, prepx);
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


char * hex2bin4(char hex){
  switch (hex){
    case '0':
      return "0000";
    case '1':
      return "0001";
    case '2':
      return "0010";
    case '3':
      return "0011";
    case '4':
      return "0100";
    case '5':
      return "0101";
    case '6':
      return "0110";
    case '7':
      return "0111";
    case '8':
      return "1000";
    case '9':
      return "1001";
    case 'a':
      return "1010";
    case 'b':
      return "1011";
    case 'c':
      return "1100";
    case 'd':
      return "1101";
    case 'e':
      return "1110";
    case 'f':
      return "1111";
  }
  return "";
}

void insert_block(char *px, char block[4], int b){
  int j;
  /*printf("blockIN: ");
  for(j = 0; j<b; j++)
    printf("%c",block[j]);
  printf("\n");*/

  for(j = 0; j < 4-b; j++)
    strcat(px,"0000");
  for(j = 0; j < b; j++)
    strcat(px, hex2bin4(block[j]));
}

char *hex2binv6(char *px){
  int i, b, j;
  char block[4];
  char *bin;
  int len;

  bin = (char*) malloc(sizeof(char) * 129);
  len = strlen(px);

  b = 0;
  for(i = 0; i < len; i++){
    if(px[i] == ':'){
      insert_block(bin,block,b);
      b = 0;
      if(px[i+1] == ':'){
        for(j = strlen(bin); j < 128; j++)
          bin[j] = '0';
        bin[128] = '\0';
        return bin;
      } 
    } else{
      block[b] = px[i];
      b++;
    }
  }
  return bin;
}
