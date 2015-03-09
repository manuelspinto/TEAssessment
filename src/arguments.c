/* arguments.c */

#include "global.h"
#include "arguments.h"


void args_print_options(void){
	printf("usage: rv <ip-version> <file> [--option]\n");
	printf("\t<ip-version>\n");
	printf("\t\t-ipv4\n");
	printf("\t\t-ipv6\n");
	printf("\troute_views.txt\n");
	printf("\t\t[--astat] Address Space Statistics\n");
	printf("\t\t[--outdata] Prepare data for Traffic Engineering Analysis\n");
	printf("\t\t[--avstat] Average statistics\n");
	printf("\tcollector.txt\n");
	printf("\t\t[--stat] Prefix Type Statistics\n");
}

int args_verify(int argc, char **argv){
	if(argc == 1){
		args_print_options();
		exit(1);
	}
	
	if(strcmp(argv[3],"--astat") == 0)
		return 1;
	else if (strcmp(argv[3],"--outdata") == 0)
		return 2;
	else if (strcmp(argv[3],"--stat") == 0)
		return 3;
	else if (strcmp(argv[3],"--avstat") == 0)
		return 4;
	else{
		printf("Not a valid option\n");
		exit(0);
	}

	return -1;
}

void args_set(char **argv, char **fname){

	if(strcmp(argv[1],"-ipv4") == 0)
		ip_version = '4';
	else if(strcmp(argv[1],"-ipv6") == 0)
		ip_version = '6';
	else{
		printf("Not a valid ip-version option\n");
		exit(0);
	}

	*fname = (char*) malloc(strlen(argv[2])*sizeof(char));
	if(*fname == NULL){
		fprintf(stderr,"ERROR: Couldn't allocate memory \n");
		exit(2);
	}

	strcpy(*fname,argv[2]);

	return;
}

