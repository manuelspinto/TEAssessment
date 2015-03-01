/* arguments.c */

#include "global.h"
#include "arguments.h"


void args_print_options(void){
	printf("usage: rv <file> [--option]\n");
	printf("\troute_views.txt\n");
	printf("\t\t[--astat] Address Space Statistics\n");
	printf("\t\t[--outdata] Prepare data for Traffic Engineering Analysis\n");
	printf("\tcollector.txt\n");
	printf("\t\t[--pstat] Prefix Type Statistics\n");
	printf("\t\t[--ndstat] Neighbor Deaggregation Statistics\n");
}

int args_verify(int argc, char **argv){
	if(argc == 1){
		args_print_options();
		exit(1);
	}
	
	if(strcmp(argv[2],"--astat") == 0)
		return 1;
	else if (strcmp(argv[2],"--outdata") == 0)
		return 2;
	else if (strcmp(argv[2],"--pstat") == 0)
		return 3;
	else if (strcmp(argv[2],"--ndstat") == 0)
		return 4;
	else{
		printf("Not a valid option\n");
		exit(0);
	}

	return -1;
}

void args_set(char **argv, char **fname){

	*fname = (char*) malloc(strlen(argv[1])*sizeof(char));
	if(*fname == NULL){
		fprintf(stderr,"ERROR: Couldn't allocate memory \n");
		exit(2);
	}

	strcpy(*fname,argv[1]);

	return;
}

