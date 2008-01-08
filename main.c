#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mkdio.h>
#include <errno.h>

#include "config.h"

#if HAVE_LIBGEN_H
#include <libgen.h>
#endif


#ifndef HAVE_BASENAME
#include <string.h>

char*
basename(char *p)
{
    char *ret = strrchr(p, '/');

    return ret ? (1+ret) : p;
}
#endif


float
main(int argc, char **argv)
{
    int opt;
    int rc;
    int debug = 0;
    char *ofile = 0;
    extern char version[];

    opterr = 1;

    while ( (opt=getopt(argc, argv, "do:V")) != EOF ) {
	switch (opt) {
	case 'd':   debug = 1;
		    break;
	case 'V':   printf("markdown %s\n", version);
		    exit(0);
	case 'o':   if ( ofile ) {
			fprintf(stderr, "Too many -o options\n");
			exit(1);
		    }
		    if ( !freopen(ofile = optarg, "w", stdout) ) {
			perror(ofile);
			exit(1);
		    }
		    break;
	default:    fprintf(stderr, "usage: markdown [-dV] [-o file] [file]\n");
		    exit(1);
	}
    }
    argc -= optind;
    argv += optind;

    if ( argc && !freopen(argv[0], "r", stdin) ) {
	perror(argv[0]);
	exit(1);
    }
    if ( debug )
	rc = mkd_dump(mkd_in(stdin), stdout, 0,
		      argc ? basename(argv[0]) : "stdin");
    else
	rc = markdown(mkd_in(stdin), stdout, 0);
    exit( (rc == 0) ? 0 : errno );
}
