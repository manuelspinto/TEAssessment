#include "global.h"

#include "search.h"
#include "file.h"
#include "arguments.h"  

int main(int argc, char *argv[]){
	FILE *fp;
  char *rv_file_name = NULL;
  int option;

  option = args_verify(argc, argv);
  args_set(argv, &rv_file_name);

  fp = file_open(rv_file_name);
  search_do(option,argv,fp);


  file_close(fp);

	return 0;
}


