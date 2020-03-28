/* Basic test of PMIx_* functionality: Init, Finalize */

#include "pmix.h"
#include <assert.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#define MAX_PID_LEN 20
#define WRITE_OUTFILE 1
#define FILEPREFIX "init_finalize_test.out."
#ifndef PATH_MAX
#   define PATH_MAX 4096
#endif

#define PMIX_CHECK(stmt)                                             \
do {                                                                 \
   int pmix_errno = (stmt);                                          \
   if (PMIX_SUCCESS != pmix_errno) {                                 \
       fprintf(stderr, "==%d== [%s:%d] PMIx call failed with %d \n", \
        getpid(), __FILE__, __LINE__, pmix_errno);                   \
       exit(EXIT_FAILURE);                                           \
   }                                                                 \
   assert(PMIX_SUCCESS == pmix_errno);                               \
} while (0)

int main(void) {

    pmix_proc_t this_proc;
    size_t ninfo = 1;

    /* initialization */
    PMIX_CHECK(PMIx_Init(&this_proc, NULL, ninfo));
    /* finalize */
    PMIX_CHECK(PMIx_Finalize(NULL, 0));
    fprintf(stdout, "==%d== Client ns %s rank %d: PMIx_Finalize success\n", getpid(), this_proc.nspace, this_proc.rank);

    if (WRITE_OUTFILE) {
        char *outfile_nm;
	size_t outfile_nm_len = 0;
        char *wdir_str;
        wdir_str = malloc(PATH_MAX);
        if (NULL == getcwd(wdir_str, PATH_MAX)) {
            fprintf(stderr, "==%d== Warning: Working directory path too long, no outfile written\n", getpid());
	    free(wdir_str);
        }
	else {
            outfile_nm_len = strlen(wdir_str) + strlen(FILEPREFIX) + MAX_PID_LEN + 1;
            outfile_nm = malloc(outfile_nm_len);
            snprintf(outfile_nm, outfile_nm_len, "%s/%s%d", wdir_str, FILEPREFIX, getpid());
            FILE *fp = fopen(outfile_nm, "w");
            fprintf(fp, "==%d== %s %d\n", getpid(),  this_proc.nspace, this_proc.rank);
            fprintf(stdout, "==%d== outfile written: %s\n", getpid(), outfile_nm);
            fclose(fp);
	    free(outfile_nm);
	    free(wdir_str);
	}
    }
    exit(EXIT_SUCCESS);
}
