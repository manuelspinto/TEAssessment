/* search.h */

void search_do(int opt, char** argv, FILE *fp);
void getCollectorFp(FILE **fp_collector, char *fname);
void getCollectorFp_ipv6(FILE **fp_collector, char *fname);

void showHelp();
void showHelpG();

