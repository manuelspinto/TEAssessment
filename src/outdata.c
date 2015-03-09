#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "outdata.h"

void search_prepare_data(FILE *fp, char* fname){
	int i;

	char *dir_rib_out = "rib_out/ipv4/";
	char fname_out[128];
	char fname_aux[128];
	char buff[BUFF_SIZE];
	FILE *fp_out;
	FILE *fp_collector[36];
	char entry;
	char as_col[AS_SIZE],as_nei[AS_SIZE],as_ori[AS_SIZE];
	char px[20], mask[10];
	int stat = 0;
	int as_entry = -1;
	strcpy(fname_out,fname);
	strcat(fname_out,".out.txt");
	fix_path(fname_out,dir_rib_out);

	fp_out = fopen(fname_out,"w");
	if(fp_out == NULL){
		fprintf(stderr,"Error: cannot open file '%s'\n",fname_out);exit(3);}

	for(i = 0; i < COLL_SIZE; i++){
		strcpy(fname_aux,fname);
		strcat(fname_aux,".");
		strcat(fname_aux,collector_asn[i]);
		strcat(fname_aux,".txt");
		fix_path(fname_aux,dir_rib_out);
		fp_collector[i] = fopen(fname_aux,"w");
		if(fp_collector[i] == NULL){
			fprintf(stderr,"Error: cannot open file '%s'\n",fname_aux);exit(3);}
	}

	printf("Preparing data...");
	while(fgets(buff, sizeof(buff), fp) != NULL){
		entry = buff[0];
		if(entry == 'P' || (entry == 'A' && buff[1] == 'S')){
			fputs(buff, fp_out);
			if (entry == 'P'){
				get_px_and_mask(buff,&px[0],&mask[0]);
			} else {
				stat = get_asns(buff,&as_ori[0],&as_col[0],&as_nei[0]);
				as_entry = get_as_entry(as_col);
				getNeighbor(buff,&as_nei[0]);
				if(stat == 0 && as_entry != -1){
					fprintf(fp_collector[as_entry],"%c %d %d %s %s %s %s %s\n"
														  ,checkPrepending(buff)
														  ,checkPathLength(buff)
														  ,checkHopLength(buff)
														  ,as_col,as_nei,as_ori,px,mask);
				}
				fputs("\n", fp_out);
			}
		}
	}


	printf("done!\nOutput file: '%s'\n",fname_out);

	for(i = 0; i < COLL_SIZE ; i++)
		file_close(fp_collector[i]);

	return;
}

void search_prepare_data_ipv6(FILE *fp, char* fname){
	int i;

	char *dir_rib_out = "rib_out/ipv6/";
	char fname_out[128];
	char fname_aux[128];
	char buff[BUFF_SIZE];
	FILE *fp_out;
	FILE *fp_collector[36];
	char entry;
	char as_col[AS_SIZE],as_nei[AS_SIZE],as_ori[AS_SIZE];
	char px[40], mask[10];
	int stat = 0;
	int as_entry = -1;
	strcpy(fname_out,fname);
	strcat(fname_out,".out.txt");
	fix_path(fname_out,dir_rib_out);

	fp_out = fopen(fname_out,"w");
	if(fp_out == NULL){
		fprintf(stderr,"Error: cannot open file '%s'\n",fname_out);exit(3);}

	for(i = 0; i < COLL_SIZE_IPV6; i++){
		strcpy(fname_aux,fname);
		strcat(fname_aux,".");
		strcat(fname_aux,collector_asn_ipv6[i]);
		strcat(fname_aux,".txt");
		fix_path(fname_aux,dir_rib_out);
		fp_collector[i] = fopen(fname_aux,"w");
		if(fp_collector[i] == NULL){
			fprintf(stderr,"Error: cannot open file '%s'\n",fname_aux);exit(3);}
	}

	printf("Preparing data...");
	while(fgets(buff, sizeof(buff), fp) != NULL){
		entry = buff[0];
		if(entry == 'P' || (entry == 'A' && buff[1] == 'S')){
			fputs(buff, fp_out);
			if (entry == 'P'){
				get_px_and_mask(buff,&px[0],&mask[0]);
			} else {
				stat = get_asns(buff,&as_ori[0],&as_col[0],&as_nei[0]);
				as_entry = get_as_entry_ipv6(as_col);
				getNeighbor(buff,&as_nei[0]);
				if(stat == 0 && as_entry != -1){
					fprintf(fp_collector[as_entry],"%c %d %d %s %s %s %s %s\n"
														  ,checkPrepending(buff)
														  ,checkPathLength(buff)
														  ,checkHopLength(buff)
														  ,as_col,as_nei,as_ori,px,mask);
				}
				fputs("\n", fp_out);
			}
		}
	}


	printf("done!\nOutput file: '%s'\n",fname_out);

	for(i = 0; i < COLL_SIZE_IPV6 ; i++)
		file_close(fp_collector[i]);

	return;
}


void get_px_and_mask(char *px_str,  char *px, char *mask){
	int i, j;

	for(i = 8, j = 0; i < strlen(px_str);i++, j++){
		if(px_str[i] == '/'){
			px[j] = '\0';
			mask[0] = px_str[i+1];
			mask[1] = px_str[i+2];
			mask[2] = '\0';
			break;
		}
		px[j] = px_str[i];
	}
}

void get_fname(char *fname_in, char *fullpath){
	int i, len, j1, j2;
	char fname[128];

	len = strlen(fullpath);

	for(i = len-1; i >= 0; i--)
		if(fullpath[i] == '/'){
			for(j1 = i+1, j2 = 0; j1 < len; j1++, j2++)
				fname[j2] = fullpath[j1];
			fname[j2] = '\0';
			break;
		}
	strcpy(fname_in, fname);
}

void fix_path(char *fullpath, char *path){
	int i, len;
	char fname[128];

	len = strlen(fullpath);

	get_fname(&fname[0], fullpath);

	for(i = 3; i < len; i++) /* ignore '../' */
		if(fullpath[i] == '/')
			fullpath[i+1] = '\0';
	strcat(fullpath,path);
	strcat(fullpath,fname);
}

int checkPathLength(char *aspath){
	int pathlen = 0;
	int len, i;

	len = strlen(aspath);

	for(i = 8; i < len; i++)
		if(aspath[i] == ' ' || aspath[i] == '\n')
			pathlen++;

	return pathlen;
}

int checkHopLength(char *aspath){
	int hoplen = 0;
	char stored[50];
	char current[50];
	int len, i, c;

	len = strlen(aspath);

	c = 0;
	stored[0] = '\0';

	for(i = 8; i < len; i++){
		if(aspath[i] == ' ' || aspath[i] == '\n'){
			current[c] = '\0';
			if(strcmp(current,stored) != 0){
				hoplen++;
				strcpy(stored,current);
			}
			c = 0;
		}else{
			current[c] = aspath[i];
			c++;
		}
	}

	return hoplen;
}

char checkPrepending(char *aspath){
	char aux[AS_SIZE];
	char asncmp[20][AS_SIZE];
	int i, j, k, ias, len;

	ias = 0;

	len = strlen(aspath);

	for(i = 8, j = 0; i < len; i++,j++){
		if(aspath[i] == ' ' || aspath[i] == '\n'){
			aux[j] = '\0';
			for(k = 0; k < ias ; k++)
				if(strcmp(aux,asncmp[k]) == 0)
					return 'P';
			strcpy(asncmp[k],aux);
			ias++;
			j = -1;
		}else
			aux[j] = aspath[i];
	}
	return 'N';
} 

int get_asns(char *aspath, char *as_ori, char *as_col, char *as_nei){
	int i, j;
	int c = 0;
	char aux[AS_SIZE];


	for(i = 8, j = 0; i < (strlen(aspath)-1);i++, j++){
		if(aspath[i] == ' '){
			aux[j] = '\0';
			if(c == 0){
				strcpy(as_col,aux);
				c = 1;
			}
			strcpy(as_nei,aux);
			j = -1;
		}else if(aspath[i] == '{' || aspath[i] == ',')
			return -1;
		else
			aux[j] = aspath[i];
	}
	aux[j] = '\0';
	strcpy(as_ori,aux);

	return 0;
}

char *getCollector(char *aspath){
	int i, j;
	char aux[AS_SIZE];
	char *as_col;

	as_col = (char*) malloc(sizeof(char) * AS_SIZE);
	as_col[0] = '\0';

	for(i = 8, j = 0; i < (strlen(aspath)-1);i++, j++){
		if(aspath[i] == ' '){
			aux[j] = '\0';
			strcpy(as_col,aux);
			return as_col;
		}else
			aux[j] = aspath[i];
	}

	return as_col;
}

void getNeighbor(char *aspath, char *nei){
	char aux[AS_SIZE];
	int len, cnt, stop, idiff, diff, over;
	int i, i2, j;

	if(checkHopLength(aspath) == 2){
		strcpy(nei,getCollector(aspath));
		return;
	}

	len = strlen(aspath);

	stop = len-1;
	cnt = 0;
	idiff = 0;
	diff = 0;
	over = 0;

	for(i = len-1; i > 7; i--){
		if(aspath[i] == ' '){
			for(i2 = i+1, j = 0; i2 < stop; i2++, j++)
				aux[j] = aspath[i2];
			aux[j] = '\0'; 
			if(cnt == 0)
				strcpy(nei,aux);
			else if(strcmp(nei,aux) != 0)
				diff++;


			if(cnt==1 && diff == 0){
				idiff = 1;
				over = 1;
			}
			else if(cnt == 2 && diff == 2)
				return;
			else if(cnt == 2 && diff == 1 && over == 0)
				idiff = 2;

			if((idiff == 1 && diff == 1) || (idiff == 2 && diff == 2)){
				strcpy(nei,aux);
				return;
			}

			strcpy(nei,aux);
			stop = i;
			cnt++;
		}
	}
}

int get_as_entry(char *asn){
	int i;

	for(i = 0; i < COLL_SIZE ; i++)
		if(strcmp(asn,collector_asn[i]) == 0)
			return i;
	return -1;
}

int get_as_entry_ipv6(char *asn){
	int i;

	for(i = 0; i < COLL_SIZE_IPV6 ; i++)
		if(strcmp(asn,collector_asn_ipv6[i]) == 0)
			return i;
	return -1;
}

