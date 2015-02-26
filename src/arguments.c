/* arguments.c */

#include "global.h"
#include "arguments.h"


void args_print_options(void){
	printf("usage: rv route_views_file.txt [--option]\n\n");
	printf("[--astat] Address Space Statistics\n");
	printf("[--ppdata] Prepare data for Traffic Engineering Analysis\n");
	printf("[--pstat] Prefix Type Statistics\n");
}

int args_verify(int argc, char **argv){
	if(argc == 1){
		args_print_options();
		exit(1);
	}
	
	if(strcmp(argv[2],"--astat") == 0)
		return 1;
	else if (strcmp(argv[2],"--ppdata") == 0)
		return 2;

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

