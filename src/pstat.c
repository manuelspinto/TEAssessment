
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

	char string[33];

	index = -1;
    TablePrint(root, NULL, &string[0], &index, &totpx, &delpx, &deapx, &lonpx, &toppx, &prepx);
    totpx = delpx + deapx + toppx + lonpx;
    printf("TOTAL: %d\n\n",totpx);
    printf("TOP: %d\t\t\t(%.2lf)\n",toppx,(((double)toppx)/((double)totpx)) * 100);
    printf("DEAGGREGATED: %d\t\t(%.2lf)\n",deapx,(((double)deapx)/((double)totpx)) * 100);
    printf("DELEGATED: %d\t\t(%.2lf)\n",delpx,(((double)delpx)/((double)totpx)) * 100);
    printf("LONELY: %d\t\t\t(%.2lf)\n\n",lonpx,(((double)lonpx)/((double)totpx)) * 100);
    printf("Prepended prefixes: %d\t(%.2lf)\n",prepx,(((double)prepx)/((double)totpx)) * 100);

	return;
}
