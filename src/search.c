/* search.c */

#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"

#include "astat.h"
#include "outdata.h"
#include "pstat.h"
#include "ndstat.h"

void search_do(int opt, char** argv, FILE *fp){
	Node *root;

	switch(opt){
		case 1 :
			search_address_space_statistics(fp);
			break;
		case 2 :
			search_prepare_data(fp,argv[1]);
			break;
		case 3 :
			root = BuildPrefixTree(fp);
			search_prefix_type_statistics(root);
			TreeClean(root);
			break;
		case 4 :
			root = BuildPrefixTree(fp);
			TreeParentSpread(root);
			search_neighbor_deaggregation_statistics(root);
			TreeClean(root);
			break;
		default :
			break;
	}
	fclose(fp);
	return;
}



