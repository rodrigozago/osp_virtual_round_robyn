/* submit.c */
#include <stdio.h>
#include "strings.h"
#include <sys/types.h>
#include <sys/stat.h>
#define MAXCMDLEN 456

main(argc, argv)
    int    argc;
    char   *argv[];
{
    char   *stud_run_file = argv[1],
           *stud_src_file =  argv[2],
           *tmp_run_file = argv[3],
           *tmp_src_file = argv[4],
           *magic_word = argv[5];
    char   command[MAXCMDLEN],
	   *handshake =
	       "f478@!023DbrqC&^$#VnIc8748234 cHgjkB*&+brw*&+ePy8734809; >,[[u";
    struct stat *bufp, buf;
    int    code;

    if ( (code = strcmp(magic_word, handshake)) != 0 )
    {
        fprintf(stderr, "submit: Bad magic word ... exit\n");
        exit(1);
    }

    /* standardize environment against break-ins */
    putenv("PATH=/usr/ucb:/usr/bin:/bin");
    putenv("IFS= \t\n");

    bufp = &buf;

    if ( (code = stat( tmp_src_file, bufp)) == 0 )
    {
        sprintf(command, "/bin/cp %s %s",tmp_src_file,stud_src_file);
	system(command);
/*
        sprintf(command, "/bin/rm -f %s",stud_run_file);
	system(command);
*/
	unlink(stud_run_file);
    }

/* append */
    sprintf(command, "/bin/cat %s >> %s", tmp_run_file, stud_run_file);
    system(command);
    chmod(stud_run_file,  00660 );
    chmod(stud_src_file,  00660 );
}
