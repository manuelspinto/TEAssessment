/* search.c */

#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"

#include "astat.h"
#include "outdata.h"
#include "pstat.h"
#include "dstat.h"

void search_do(int opt, char** argv, FILE *fp){
	Node *root;
	char s_opt = '\0';

	switch(opt){
		case 1 :
			if(ip_version == '6')
				search_address_space_statistics_ipv6(fp);
			else
				search_address_space_statistics(fp);
			break;
		case 2 :
			if(ip_version == '6')
				search_prepare_data_ipv6(fp,argv[2]);
			else
				search_prepare_data(fp,argv[2]);
			break;
		case 3 :
			printf("Building Prefix Tree...");
			if(ip_version == '6')
				root = BuildPrefixTree_ipv6(fp);
			else
				root = BuildPrefixTree(fp);
			printf("done!\nSpreading Parent Information...");
			TreeParentSpread(root);
			printf("done!\n");
			showHelp();
			while(1){
				printf(":");
				scanf("%c",&s_opt);
				if(s_opt == 'q')
					break;
				switch(s_opt){
					case 'p':
						search_prefix_type_statistics(root);
						break;
					case 'n':
						search_neighbor_deaggregation_statistics(root);
						break;
					case 'l':
						search_deaggregation_length_statistics(root);
						break;
					case 'h':
						showHelp();
						break;
					default:
						break;
				}
			}
			TreeClean(root);
			break;
		default :
			break;
	}
	fclose(fp);
	return;
}

void showHelp(void){
	printf("Help:");
	printf("\tp - Prefix Type Statistics\n");
	printf("\tn - Neighbor Statistics\n");
	printf("\tl - Path length Statistics\n\n");
	printf("\th - Show help\n");
	printf("\tq - quit\n");
}



