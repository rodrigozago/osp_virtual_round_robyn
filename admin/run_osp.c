/* run_osp.c */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <pwd.h>

#define TRUE  1
#define FALSE 0
#define SHARP_LINE "##############################################################################\n\n"

#define MAXPATHLEN  256
#define MAXCMDLEN   456
#define LINELEN	    100

char  *assignment_path,
      *module_list,
      *computer_architecture,
      *handshake = 
	    "'f478@!023DbrqC&^$#VnIc8748234 cHgjkB*&+brw*&+ePy8734809; >,[[u'",
      student_run_file[MAXPATHLEN],
      student_src_file[MAXPATHLEN],
      tmp_run_file[MAXPATHLEN],
      tmp_src_file[MAXPATHLEN];


main(argc, argv)
    int    argc;
    char   *argv[];
{
    FILE   *run_fp;

    struct passwd *getpwuid();
    struct stat *bufp, buf;
    long   timeofday, time();
    int    stat_code;
    int	   pid = getpid();   /*  used as a file name */
    char   *getlogin(), *index();
    char 
	  *whoami = getpwuid(getuid())->pw_name,
	  compiler[30],
	  response[80],
	  user_name[80],
	  *date,
	  command[MAXCMDLEN];


    user_name[0] = command[0] = '\0';

    computer_architecture = index( argv[0], '.');
	   /* get computer architecture gen is running on      */
	   /* run_osp should have the name "run_osp.architecture-name" */
	   /* index finds the last occurrence of '.' */

    assignment_path = argv[1];
    module_list = argv[2];

    sprintf( student_run_file, "%s/submissions/%s.run", assignment_path,
							   whoami );
    sprintf( student_src_file, "%s/submissions/%s.src", assignment_path,
							   whoami );
    sprintf( tmp_run_file, "/tmp/.%s.run%d", whoami, pid);
    sprintf( tmp_src_file, "/tmp/.%s.src%d", whoami, pid);

    bufp = &buf;
    if ( (stat_code = stat( student_run_file, bufp)) == 0 )
    {
	printf("\n ***  %s  has submitted this assignment on  %s\n",
					whoami, ctime ( &(bufp->st_mtime)) );
	printf("To quit hit <Return>; To overwrite type 'o' : ");
	
	gets(response);
	if ( strcmp(response, "o") != 0 )
		exit(1);
    }

    printf("\nEnter author(s) names : ");
    gets(user_name);

    timeofday = time( (long *) 0);
    date = ctime( &timeofday);

    system("/bin/rm -f simulation.run .sim.run");
    sprintf(command, "/bin/rm -f %s %s " ,tmp_src_file, tmp_run_file);
    system(command);

    if ( (run_fp = fopen( "simulation.run" , "w")) == NULL ) {
	printf("\nCan't open 'simulation.run'; NFS does not respond?\n\n");
	exit(-1);
    }


    fprintf(run_fp,"******************************************************************************\n\n");
    fprintf(run_fp,"               Student Name(s) : %s\n\n",user_name);
    fprintf(run_fp,"               Hand In Time    : %s\n\n",date);
    fprintf(run_fp,"******************************************************************************\n\n");

    printf("\nWhich C compiler? (cc, CC, gcc, etc.)   cc : ");
    gets(compiler);
    if (empty(compiler))
	strcpy(compiler, "cc");
    printf("\nCompiler = %s\n\n", compiler);

    printf("\n... compiling OSP ...\n");
    fprintf(run_fp,"\n\n**************   COMPILATION MESSAGES   **************\n\n");
    fclose(run_fp);

    sprintf(command,"touch *.c; make -f %s/Makefile C_COMP=\"%s\" 1> .compile_msgs 2>&1;/bin/cat .compile_msgs",
						    assignment_path, compiler);
    system(command);

    sprintf(command, "/bin/cat .compile_msgs >> %s; /bin/rm -f .compile_msgs", "simulation.run" );
    system(command);

    if ( (run_fp = fopen("simulation.run","a")) == NULL ) {
	printf("\nCan't open 'simulation.run'; NFS does not respond?\n\n");
	exit(-1);
    }

    fprintf(run_fp,"\n\n************   COMPILATION MESSAGES END   ************\n\n");
    fclose(run_fp);

	/* submit this part before one changes it */

    sprintf(command, "/bin/cat simulation.run > .sim.run" );
    system(command);

    chmod("simulation.run", 00644);
    sprintf(command, "/bin/cat %s  > %s" , module_list, tmp_src_file);
    system(command);
    sprintf(command, "/bin/cat simulation.run  > %s" , tmp_run_file);
    system(command);

    chmod(tmp_src_file , 00644);
    chmod(tmp_run_file , 00644);

    sprintf(command,"%s/../bin/submit%s %s %s %s %s %s",
					 assignment_path, computer_architecture,
					 student_run_file, student_src_file,
					 tmp_run_file, tmp_src_file, handshake);
    system(command);

    sprintf(command, "/bin/rm -f %s", tmp_src_file);
    system(command);

    now_run_osp();

    sprintf(command, "/bin/rm -f %s", tmp_run_file);
    system(command);
    system( "/bin/mv -f .sim.run simulation.run");
    chmod ("simulation.run", 00600);

    sprintf(command, "/bin/rm -f OSP *.o " );
    system(command);
		/* delete student's executable and .o files after submission */

    printf("\n\nYour assignment has just been submitted. \n");
    printf("\nPlease check for any compile-time or run-time error messages.\n");
    printf("Also check that the copy of your submission in `./simulation.run' is OK.\n\n");

    printf("\n\nNote: all .o files have been removed to prevent disk overflow...\n\n\n");
}


/* run OSP */
now_run_osp( )
{
    char   parameter_file_name[MAXPATHLEN],
	   instructor_parameter_file_name[MAXPATHLEN],
	   command[MAXCMDLEN];
    FILE   *output_fp, *par_file;
    struct stat buf;
    int    stat_code;
    char   *gets_code;

    command[0]=parameter_file_name[0]=instructor_parameter_file_name[0]= '\0';

    printf("\n\nEnter parameter file name (hit <Return> to quit) : ");
    gets_code = gets(parameter_file_name);

    while (*parameter_file_name != '\0') {

	if (gets_code == NULL) {
	    printf("\n\n*******EOF on 'stdin' ... exit ... call again!\n\n");
	    exit(-1);
	}

	if ( (output_fp = fopen("simulation.run","w")) == NULL ) {
	    printf("\nCan't open 'simulation.run'; NFS does not respond?\n\n");
	    exit(-1);
	}

	fprintf(output_fp,"\n\n\n\n\n\n");

	sprintf(instructor_parameter_file_name,"%s/%s",assignment_path,parameter_file_name);

	if ( (stat_code = stat(instructor_parameter_file_name, &buf)) != 0 )
	{		/*  private parameter file */
	    fprintf(output_fp, SHARP_LINE);
	    fprintf(output_fp,"\t\t\tExecuting OSP with  '%s'\n\n",
							parameter_file_name);
	    fprintf(output_fp,"\t   This parameter file has been chosen by the ** STUDENT **\n\n");
	    fprintf(output_fp, SHARP_LINE);

	    if ((par_file = fopen(parameter_file_name, "r")) == NULL)  {
		printf("\n***parameter file '%s' does not exist or is unreadable...\n\n", parameter_file_name);
		fprintf(output_fp, "\n***parameter file '%s' does not exist or is unreadable...\n\n", parameter_file_name);
		goto ask_again;
 	    }
	    else
		fclose(par_file);

	    sprintf(command,"OSP -a -d %s ", parameter_file_name);
	}
	else
	{		/*  instructor's parameter file */
	    fprintf(output_fp, SHARP_LINE);
	    fprintf(output_fp,"\t\t\tExecuting OSP with\n\t\t'%s'\n\n",
						instructor_parameter_file_name);
	    fprintf(output_fp,"\t   This parameter file has been given by the INSTRUCTOR\n\n");
	    fprintf(output_fp, SHARP_LINE);

	    if ((par_file = fopen(instructor_parameter_file_name, "r")) == NULL)  {
		printf("\n***parameter file '%s'\ndoes not exist or is unreadable by non-owner... contact the instructor\n\n", instructor_parameter_file_name);
		fprintf(output_fp, "\n***parameter file '%s'\ndoes not exist or is unreadable by non-owner... contact the instructor\n\n", instructor_parameter_file_name);
		goto ask_again;
 	    }
	    else
		fclose(par_file);

	    sprintf(command,"OSP -a -d %s ",instructor_parameter_file_name);
	}

	get_comments(output_fp);
	fclose(output_fp);
	system(command);

	sprintf(command, "/bin/cat simulation.run >> .sim.run" );
	system(command);
	chmod("simulation.run", 00644);

	sprintf(command, "/bin/cp simulation.run  %s" , tmp_run_file);
	system(command);

	sprintf(command, "%s/../bin/submit%s %s %s %s %s %s",
					 assignment_path, computer_architecture,
					 student_run_file, student_src_file,
					 tmp_run_file, tmp_src_file, handshake);
	system(command);

ask_again:
	printf("\n\n\nEnter parameter file name (hit <Return> to quit) : ");
	gets_code = gets(parameter_file_name);
    }

}


get_comments(file)

FILE  *file;
{
   char comments[LINELEN];

	printf("\nEnter comments, if needed ... <Return> ends comments\n\n");

	fprintf( file, "\n\n%s\t\t\t\tCOMMENTS\n\n\n", SHARP_LINE);
	printf("COMMENT> ");

	while ( (strcmp(gets( comments ), "") != 0 )
		&& (comments != NULL) )     {

		fprintf(file, "%s\n", comments);
		printf("COMMENT> ");
	}

	printf("\n\n");
	fprintf( file, "\n\n%s\n", SHARP_LINE);
}


empty(string)
char *string;
{
	while ( *string != '\0' )
	    if ( !isspace(*string) )
		return(FALSE);

	return(TRUE);
}



