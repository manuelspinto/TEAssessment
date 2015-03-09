#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "astat.h"

void search_address_space_statistics(FILE *fp){
	int total_entries = 0;
	int mask_count[MASK_SIZE_ipv4 + 1];
	int i;
	char buff[BUFF_SIZE];
	
	int mask;

	int mask_g_24 = 0;
	int mask_24 = 0;
	int mask_17_23 = 0;
	int mask_16 = 0;
	int mask_8_15 = 0;
	int mask_l_8 = 0;

	for(i = 0; i <= MASK_SIZE_ipv4; i++)
		mask_count[i] = 0;

	while(fgets(buff, sizeof(buff), fp) != NULL){
		if(buff[0] == 'P'){
			mask = get_mask(buff);
			mask_count[mask]++;
			total_entries++;
		}
	}

	for(i = 25; i <= MASK_SIZE_ipv4 ; i++)
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

void search_address_space_statistics_ipv6(FILE *fp){
	int total_entries = 0;
	int mask_count[MASK_SIZE_ipv6 + 1];
	int i;
	char buff[BUFF_SIZE];
	
	int mask;

	int mask_g_64 = 0;
	int mask_64 = 0;
	int mask_49_63 = 0;
	int mask_48 = 0;
	int mask_33_47 = 0;
	int mask_32 = 0;
	int mask_17_31 = 0;
	int mask_16 = 0;
	int mask_l_16 = 0;

	for(i = 0; i <= MASK_SIZE_ipv6; i++)
		mask_count[i] = 0;

	while(fgets(buff, sizeof(buff), fp) != NULL){
		if(buff[0] == 'P'){
			mask = get_mask(buff);
			mask_count[mask]++;
			total_entries++;
		}
	}

	for(i = 65; i <= MASK_SIZE_ipv6 ; i++)
		mask_g_64 += mask_count[i];
	mask_64 = mask_count[64];
	for(i = 49; i <= 63  ; i++)
		mask_49_63 += mask_count[i];
	mask_48 = mask_count[48];
	for(i = 33; i <= 47  ; i++)
		mask_33_47 += mask_count[i];
	mask_32 = mask_count[32];
	for(i = 17; i <= 31  ; i++)
		mask_17_31 += mask_count[i];
	mask_16 = mask_count[16];
	for(i = 1; i <= 15  ; i++)
		mask_l_16 += mask_count[i];

	printf("Address Space Statistics:\n");

	printf("/>64\t%.2lf%%\t(%d)\n",		100*((double) mask_g_64) / ((double) total_entries), mask_g_64);;
	printf("/64\t%.2lf%%\t(%d)\n",		100*((double) mask_64) / ((double) total_entries), mask_64);
	printf("/49-/63\t%.2lf%%\t(%d)\n",	100*((double) mask_49_63) / ((double) total_entries), mask_49_63);
	printf("/48\t%.2lf%%\t(%d)\n",		100*((double) mask_48) / ((double) total_entries), mask_48);
	printf("/33-/47\t%.2lf%%\t(%d)\n",	100*((double) mask_33_47) / ((double) total_entries), mask_33_47);
	printf("/32\t%.2lf%%\t(%d)\n",		100*((double) mask_32) / ((double) total_entries), mask_32);
	printf("/17-/31\t%.2lf%%\t(%d)\n",	100*((double) mask_17_31) / ((double) total_entries), mask_17_31);
	printf("/16\t%.2lf%%\t(%d)\n",		100*((double) mask_16) / ((double) total_entries), mask_16);
	printf("/<16\t%.2lf%%\t(%d)\n",		100*((double) mask_l_16) / ((double) total_entries), mask_l_16);

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



