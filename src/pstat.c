
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
	int topdea = 0;
	int deltpx = 0;
	int prepx = 0;

	int *preptype;
	int i;

	char string[129];


	preptype = (int*) malloc(sizeof(int)*5);
	for(i=0;i<6;i++)preptype[i]=0;


	index = -1;
    TablePrint(root, NULL, &string[0], &index, &totpx, &delpx, &deltpx, &deapx, &lonpx, &toppx, &topdea, &prepx, preptype);
    totpx = delpx + deapx + toppx + lonpx + deltpx + topdea;
    printf("TOTAL:\t\t%d\n\n",totpx);
    printf("TOP-DEL:\t%d\t(%.2lf%%)\t\t%d\t%.2lf%%\t%.2lf%%\n",toppx,(((double)toppx)/((double)totpx)) * 100
    												  ,preptype[0], (((double)preptype[0])/((double)toppx)) * 100, (((double)preptype[0])/((double)totpx)) * 100);

    printf("TOP-DEA:\t%d\t(%.2lf%%)\t\t%d\t%.2lf%%\t%.2lf%%\n",topdea,(((double)topdea)/((double)totpx)) * 100
    												  ,preptype[5], (((double)preptype[5])/((double)topdea)) * 100, (((double)preptype[5])/((double)totpx)) * 100);
    
    printf("DEL-TOP:\t%d\t(%.2lf%%)\t\t%d\t%.2lf%%\t%.2lf%%\n",deltpx,(((double)deltpx)/((double)totpx)) * 100
    												  ,preptype[3], (((double)preptype[3])/((double)deltpx)) * 100, (((double)preptype[3])/((double)totpx)) * 100);
    
    printf("LON:\t\t%d\t(%.2lf%%)\t%d\t%.2lf%%\t%.2lf%%\n",lonpx,(((double)lonpx)/((double)totpx)) * 100
    												  ,preptype[1], (((double)preptype[1])/((double)lonpx)) * 100, (((double)preptype[1])/((double)totpx)) * 100);
    
    printf("DEL-LON:\t%d\t(%.2lf%%)\t%d\t%.2lf%%\t%.2lf%%\n",delpx,(((double)delpx)/((double)totpx)) * 100
    												  ,preptype[4], (((double)preptype[4])/((double)delpx)) * 100, (((double)preptype[4])/((double)totpx)) * 100);
    
    printf("DEA:\t\t%d\t(%.2lf%%)\t%d\t%.2lf%%\t%.2lf%%\n\n",deapx,(((double)deapx)/((double)totpx)) * 100
    												  ,preptype[2], (((double)preptype[2])/((double)deapx)) * 100, (((double)preptype[2])/((double)totpx)) * 100);
    
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
	int topdea = 0;
	int deltpx = 0;
	int prepx = 0;

	int *preptype;
	int i;

	char string[129];


	preptype = (int*) malloc(sizeof(int)*5);
	for(i=0;i<6;i++)preptype[i]=0;


	index = -1;
    TablePrint_ipv6(root, NULL, &string[0], &index, &totpx, &delpx, &deltpx, &deapx, &lonpx, &toppx, &topdea, &prepx, preptype);
    totpx = delpx + deapx + toppx + lonpx + deltpx + topdea;
    printf("TOTAL:\t\t%d\n\n",totpx);
    printf("TOP-DEL:\t%d\t(%.2lf%%)\t\t%d\t%.2lf%%\t%.2lf%%\n",toppx,(((double)toppx)/((double)totpx)) * 100
    												  ,preptype[0], (((double)preptype[0])/((double)toppx)) * 100, (((double)preptype[0])/((double)totpx)) * 100);

    printf("TOP-DEA:\t%d\t(%.2lf%%)\t\t%d\t%.2lf%%\t%.2lf%%\n",topdea,(((double)topdea)/((double)totpx)) * 100
    												  ,preptype[5], (((double)preptype[5])/((double)topdea)) * 100, (((double)preptype[5])/((double)totpx)) * 100);
    
    printf("DEL-TOP:\t%d\t(%.2lf%%)\t\t%d\t%.2lf%%\t%.2lf%%\n",deltpx,(((double)deltpx)/((double)totpx)) * 100
    												  ,preptype[3], (((double)preptype[3])/((double)deltpx)) * 100, (((double)preptype[3])/((double)totpx)) * 100);
    
    printf("LON:\t\t%d\t(%.2lf%%)\t%d\t%.2lf%%\t%.2lf%%\n",lonpx,(((double)lonpx)/((double)totpx)) * 100
    												  ,preptype[1], (((double)preptype[1])/((double)lonpx)) * 100, (((double)preptype[1])/((double)totpx)) * 100);
    
    printf("DEL-LON:\t%d\t(%.2lf%%)\t%d\t%.2lf%%\t%.2lf%%\n",delpx,(((double)delpx)/((double)totpx)) * 100
    												  ,preptype[4], (((double)preptype[4])/((double)delpx)) * 100, (((double)preptype[4])/((double)totpx)) * 100);
    
    printf("DEA:\t\t%d\t(%.2lf%%)\t%d\t%.2lf%%\t%.2lf%%\n\n",deapx,(((double)deapx)/((double)totpx)) * 100
    												  ,preptype[2], (((double)preptype[2])/((double)deapx)) * 100, (((double)preptype[2])/((double)totpx)) * 100);
    
    printf("Prepended:\t%d\t(%.2lf%%)\n",prepx,(((double)prepx)/((double)totpx)) * 100);

	return;
}

void printTree(Node *root){
	int ret = 0;
	int topcnt = 0;

    TopTreePrint(root, NULL, &ret, &topcnt);

	return;
}


