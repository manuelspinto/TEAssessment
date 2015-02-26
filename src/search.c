/* search.c */

#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"

void search_do(int opt, char** argv, FILE *fp){
	switch(opt){
		case 1 :
			search_address_space_statistics(fp);
			break;
		case 2 :
			search_prepare_data(fp,argv[1]);
			break;
		default :
			break;
	}
	return;
}

void search_address_space_statistics(FILE *fp){
	int total_entries = 0;
	int mask_count[33];
	int i;
	char buff[BUFF_SIZE];
	
	int mask;

	int mask_g_24 = 0;
	int mask_24 = 0;
	int mask_17_23 = 0;
	int mask_16 = 0;
	int mask_8_15 = 0;
	int mask_l_8 = 0;

	for(i = 0; i <= 32; i++)
		mask_count[i] = 0;

	while(fgets(buff, sizeof(buff), fp) != NULL){
		if(buff[0] == 'P'){
			mask = get_mask(buff);
			mask_count[mask]++;
			total_entries++;
		}
	}

	for(i = 25; i <= 32 ; i++)
		mask_g_24 += mask_count[i];
	mask_24 = mask_count[24];
	for(i = 17; i <= 23  ; i++)
		mask_17_23 += mask_count[i];
	mask_16 = mask_count[16];
	for(i = 8; i <= 15  ; i++)
		mask_8_15 += mask_count[i];

		mask_l_8 += mask_count[8];
	

	printf("Address Space Statistics:\n");

	printf("/>24\t%.2lf%%\t(%d)\n",		100*((double) mask_g_24) / ((double) total_entries), mask_g_24);
	printf("/24\t%.2lf%%\t(%d)\n",		100*((double) mask_24) / ((double) total_entries), mask_24);
	printf("/17-/23\t%.2lf%%\t(%d)\n",	100*((double) mask_17_23) / ((double) total_entries), mask_17_23);
	printf("/16\t%.2lf%%\t(%d)\n",		100*((double) mask_16) / ((double) total_entries), mask_16);
	printf("/8-/15\t%.2lf%%\t(%d)\n",	100*((double) mask_8_15) / ((double) total_entries), mask_8_15);
	printf("/<8\t%.2lf%%\t(%d)\n",		100*((double) mask_l_8) / ((double) total_entries), mask_l_8);

	return;
}

int get_mask(char *px_str){
	int i;
	char mask[3];

	for(i = 0; i < strlen(px_str);i++)
		if(px_str[i] == '/'){
			mask[0] = px_str[i+1];
			mask[1] = px_str[i+2];
			mask[2] = '\0';
		}

	return atoi(mask);
}

void search_prepare_data(FILE *fp, char* fname){
	int i;

	int index = 0;
	int totpx = 0;
	int delpx = 0;
	int deapx = 0;
	int lonpx = 0;
	int toppx = 0;
	char string[33];

	char *dir_rib_out = "rib_out/";
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
	Line line;
	Node * root = NodeNew();

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
				if(stat == 0 && as_entry != -1){
					fprintf(fp_collector[as_entry],"%s %s %s %s %s\n"
														  ,as_col,as_nei,as_ori,px,mask);
					
					if(strcmp(as_col,"3549") == 0){
						strcpy(line.ip, dec2bin(px));
	    				line.mask = atoi(mask);
	    				strcpy(line.asn, as_ori);
	    				TreeInsert(root,line);
    				}

				}
				fputs("\n", fp_out);
			}
		}
	}
	TreeSpread(root,NULL);
	index = -1;
    TablePrint(root, NULL, &string[0], &index, &totpx, &delpx, &deapx, &lonpx, &toppx);
    totpx = delpx + deapx + toppx + lonpx;
    printf("\n\nTOTAL: %d\n\n",totpx);
    printf("TOP: %d\t\t\t(%.2lf)\n",toppx,(((double)toppx)/((double)totpx)) * 100);
    printf("DEAGGREGATED: %d\t\t(%.2lf)\n",deapx,(((double)deapx)/((double)totpx)) * 100);
    printf("DELEGATED: %d\t\t(%.2lf)\n",delpx,(((double)delpx)/((double)totpx)) * 100);
    printf("LONELY: %d\t\t\t(%.2lf)\n\n",lonpx,(((double)lonpx)/((double)totpx)) * 100);

    TreeClean(root);


	printf("done!\nOutput file: '%s'\n",fname_out);

	file_close(fp_out);
	for(i = 0; i < COLL_SIZE ; i++)
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

	for(i = 2; i < len; i++) /* ignore './' */
		if(fullpath[i] == '/')
			fullpath[i+1] = '\0';
	strcat(fullpath,path);
	strcat(fullpath,fname);
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

int get_as_entry(char *asn){
	int i;

	for(i = 0; i < COLL_SIZE ; i++)
		if(strcmp(asn,collector_asn[i]) == 0)
			return i;
	
	return -1;
}



