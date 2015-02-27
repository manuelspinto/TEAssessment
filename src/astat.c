#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "astat.h"

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


