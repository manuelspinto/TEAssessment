
#include "global.h"
#include "search.h"
#include "file.h"
#include "tree.h"
#include "pstat.h"

void search_prefix_type_statistics(FILE *fp, char* fname){

	int index = 0;
	int totpx = 0;
	int delpx = 0;
	int deapx = 0;
	int lonpx = 0;
	int toppx = 0;
	char string[33];
	
	char buff[BUFF_SIZE];
	char px[20], mask[10];
	char as_ori[AS_SIZE];
	Line line;
	Node * root = NodeNew();

	while(fgets(buff, sizeof(buff), fp) != NULL){
		buff[strlen(buff)-1] = '\0';
		get_ori_px_mask(buff,&as_ori[0],&px[0],&mask[0]);

		strcpy(line.ip, dec2bin(px));
    	line.mask = atoi(mask);
    	strcpy(line.asn, as_ori);
    	TreeInsert(root,line);
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
    fclose(fp);

	return;
}

void get_ori_px_mask(char *buff, char *as_ori, char *px, char *mask){
	char dummy[AS_SIZE];

	sscanf(buff,"%s %s %s %s %s", dummy, dummy, as_ori, px, mask);

}
