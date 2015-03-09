

void search_prepare_data(FILE *fp, char* fname);
void search_prepare_data_ipv6(FILE *fp, char* fname);

void get_px_and_mask(char *px_str,  char *px, char *mask);
int  get_asns(char *aspath, char *as_ori, char *as_col, char *as_nei);
int  get_as_entry(char *asn);
int get_as_entry_ipv6(char *asn);
void get_fname(char *fname_in, char *fullpath);
void fix_path(char *fullpath, char *path);

char checkPrepending(char *aspath);
int  checkPathLength(char *aspath);
int  checkHopLength(char *aspath);
void getNeighbor(char *aspath, char *nei);
char *getCollector(char *aspath);

