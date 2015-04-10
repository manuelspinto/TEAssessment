/* search.c */

#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"

#include "astat.h"
#include "outdata.h"
#include "pstat.h"
#include "dstat.h"
#include "gstat.h"

void search_do(int opt, char** argv, FILE *fp){
	Node *root;
	int i;
	char s_opt = '\0';
	FILE *fp_ipv4[36];
	FILE *fp_ipv6[17];
	Node *root_ipv4[36];
	Node *root_ipv6[17];

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
			ChildSpread(root);
			printf("done!\n");
			showHelp();
			while(1){
				printf(":");
				scanf("%c",&s_opt);
				if(s_opt == 'q')
					break;
				switch(s_opt){
					case 'p':
						if(ip_version == '6')
							search_prefix_type_statistics_ipv6(root);
						else
							search_prefix_type_statistics(root);
						break;
					case 'n':
						search_neighbor_deaggregation_statistics(root);
						break;
					case 'l':
						search_deaggregation_length_statistics(root);
						break;
					case 't':
						printTree(root);
						break;
					case 'd':
						search_deaggregation_statistics(root);
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
		case 4 :
			getCollectorFp(fp_ipv4, argv[2]);

			printf("Building Prefixes Tree...");
			if(ip_version == '6'){
				for(i = 0; i < 17 ; i++){
					root_ipv6[i] = BuildPrefixTree_ipv6(fp_ipv6[i]);
					TreeParentSpread(root_ipv6[i]);
					ChildSpread(root_ipv6[i]);
				}
			}
			else{
				for(i = 0; i < 36 ; i++){
					root_ipv4[i] = BuildPrefixTree(fp_ipv4[i]);
					TreeParentSpread(root_ipv4[i]);
					ChildSpread(root_ipv4[i]);
					checkScopAndPrep(root_ipv4[i]);
					fprintf(stdout,":%.0lf%%\n",(double)(((double)i)/ 36) * 100);
				}
			}
			printf("done!\n");
			printf("Filling Master root...\n");
			masterFill(root_ipv4);
			printf("done\n");
			showHelpG();
			while(1){
				printf(":");
				scanf("%c",&s_opt);
				if(s_opt == 'q')
					break;
				switch(s_opt){
					case 'n':
						search_neighbor_deaggregation_statistics(root_ipv4[0]);
						break;
					case 'd':
						search_deaggregation_statistics(root_ipv4[0]);
						break;
					case 't':
						printf("\n**** One collector\n");
						search_te_statistics(root_ipv4[1]);
						printf("\n\n****All collectors\n\n");
						search_te_statistics(root_ipv4[0]);
						break;
					case 'p':
						printf("\n**** One collector\n");
							search_prefix_type_statistics(root_ipv4[1]);
						printf("\n\n****All collectors\n\n");
							search_prefix_type_statistics(root_ipv4[0]);
						break;
					case 's':
						printTopTree(root_ipv4[0]);
						break;
					case 'h':
						showHelpG();
						break;
					default:
						break;
				}
			}
			printf("Cleaning Trees...\n");
			if(ip_version == '6')
				for(i = 0; i < 17 ; i++)
					TreeClean(root_ipv6[i]);
			else
				for(i = 0; i < 36 ; i++){
					TreeClean(root_ipv4[i]);
					fprintf(stdout,":%.0lf%%\n",(double)(((double)i)/ 36) * 100);
				}
			printf("done!\n");
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
	printf("\tl - Path length Statistics\n");
	printf("\td - Deaggregation Level\n");
	printf("\tt - PrintTopTree\n\n");
	printf("\th - Show help\n");
	printf("\tq - quit\n");
}
void showHelpG(void){
	printf("Help:");
	printf("\tn - Neighbor Statistics\n");
	printf("\td - Deaggregation Level\n\n");
	printf("\th - Show help\n");
	printf("\tq - quit\n");
}

void getCollectorFp(FILE **fp_collector, char *fname){
	int i;
	char *dir_rib_out = "rib_out/ipv4/";
	char fname_aux[128];

	for(i = 0; i < COLL_SIZE; i++){
		strcpy(fname_aux,fname);
		strcat(fname_aux,".");
		strcat(fname_aux,collector_asn[i]);
		strcat(fname_aux,".txt");
		fix_path(fname_aux,dir_rib_out);
		fp_collector[i] = fopen(fname_aux,"r");
		if(fp_collector[i] == NULL){
			fprintf(stderr,"Error: cannot open file '%s'\n",fname_aux);exit(3);}
	}
}



