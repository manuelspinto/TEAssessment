
#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "pstat.h"

void search_prefix_type_statistics(Node *root){

	int index = 0;
	int totpx = 0;
	int delpx = 0;
	int deapx = 0;
	int lonpx = 0;
	int toppx = 0;
	int prepx = 0;

	char string[129];

	index = -1;
    TablePrint(root, NULL, &string[0], &index, &totpx, &delpx, &deapx, &lonpx, &toppx, &prepx);
    totpx = delpx + deapx + toppx + lonpx;
    printf("TOTAL:\t\t%d\n\n",totpx);
    printf("TOP:\t\t%d\t(%.2lf%%)\n",toppx,(((double)toppx)/((double)totpx)) * 100);
    printf("DEAGGREGATED:\t%d\t(%.2lf%%)\n",deapx,(((double)deapx)/((double)totpx)) * 100);
    printf("DELEGATED:\t%d\t(%.2lf%%)\n",delpx,(((double)delpx)/((double)totpx)) * 100);
    printf("LONELY:\t\t%d\t(%.2lf%%)\n\n",lonpx,(((double)lonpx)/((double)totpx)) * 100);
    printf("Prepended:\t%d\t(%.2lf%%)\n",prepx,(((double)prepx)/((double)totpx)) * 100);

	return;
}

void search_prefix_type_statistics_ipv6(Node *root){

	int index = 0;
	int totpx = 0;
	int delpx = 0;
	int deapx = 0;
	int lonpx = 0;
	int toppx = 0;
	int prepx = 0;

	char string[129];

	index = -1;
    TablePrint_ipv6(root, NULL, &string[0], &index, &totpx, &delpx, &deapx, &lonpx, &toppx, &prepx);
    totpx = delpx + deapx + toppx + lonpx;
    printf("TOTAL:\t\t%d\n\n",totpx);
    printf("TOP:\t\t%d\t(%.2lf%%)\n",toppx,(((double)toppx)/((double)totpx)) * 100);
    printf("DEAGGREGATED:\t%d\t(%.2lf%%)\n",deapx,(((double)deapx)/((double)totpx)) * 100);
    printf("DELEGATED:\t%d\t(%.2lf%%)\n",delpx,(((double)delpx)/((double)totpx)) * 100);
    printf("LONELY:\t\t%d\t(%.2lf%%)\n\n",lonpx,(((double)lonpx)/((double)totpx)) * 100);
    printf("Prepended:\t%d\t(%.2lf%%)\n",prepx,(((double)prepx)/((double)totpx)) * 100);

	return;
}

void printTree(Node *root){
	int ret = 0;
	int topcnt = 0;

    TopTreePrint(root, NULL, &ret, &topcnt);

	return;
}


