/* file.c */

#include "global.h"
#include "file.h"

extern char collector_asn[COLL_SIZE][10];

FILE *file_open(char *fname){
	FILE *fp;
	
	fp = fopen(fname,"r");
  	if(fp == NULL){
	    fprintf(stderr,"ERROR: Can't open file '%s'\n",fname);
	    exit(1);
 	}

 	printf("File: '%s'\n",fname);	

	return fp;
}

void file_close(FILE *fp){
	fclose(fp);
}

FILE *file_reset(FILE*fp, char *fname){
	file_close(fp);
	fp = file_open(fname);
	return fp;
}

char **file_malloc(FILE *fp, char *fname){
	int file_len = 0;
	char buff[256];
	char **buffer;
	int id, id2;


	printf("Extracting number of lines...\n");
	/*while (fgets(buff, sizeof(buff), fp) != NULL )*/
		/*file_len+=strlen(buff); fgets -> + \n em valor é igual a + '\0'*/
		/*file_len++;*/

	printf("#%d lines\n",file_len);
	
	/*file_len = 4794904240;*/
	file_len = 202005559;

	/*fp = file_reset(fp, fname);*/

	id = 0;
	id2 = 0;
	buffer = (char**) malloc(file_len*sizeof(char));
	if(buffer == NULL){
			fprintf(stderr,"ERROR: Not enough memory\n");
			exit(2);
	}

	printf("Filling buffer....\n");
	while (fgets(buff, sizeof(buff), fp) != NULL ){
		buffer[id] = (char*) malloc(strlen(buff)*sizeof(char));
		if(buffer[id] == NULL){
			fprintf(stderr,"ERROR: Not enough memory\n");
			exit(2);
		}
		strcpy(buffer[id],buff);
		id++;
		id2++;

		if(id2 == 1000000){
			id2 = 0;
			printf("%lf\n",(((double)id)/((double) file_len)) * 100);
		}
		
	}
	printf("DONE!\n");


	return buffer;
}


