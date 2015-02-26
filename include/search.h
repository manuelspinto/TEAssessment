/* search.h */

void search_do(int opt, char** argv, FILE *fp);

void search_address_space_statistics(FILE *fp);
void search_prepare_data(FILE *fp, char* fname);

int get_mask(char *px_str);
void get_px_and_mask(char *px_str,  char *px, char *mask);
int get_asns(char *aspath, char *as_ori, char *as_col, char *as_nei);
int get_as_entry(char *asn);
void get_fname(char *fname_in, char *fullpath);
void fix_path(char *fullpath, char *path);



