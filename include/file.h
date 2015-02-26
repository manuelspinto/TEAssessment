/* file.h */


FILE *file_open(char *fname);
void file_close(FILE *fp);
char **file_malloc(FILE *fp, char *fname);



