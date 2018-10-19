/****************************************************************************/
/*                               gen.c					    */
/* 									    */
/*   								 	    */
/*  									    */
/*   									    */
/*             								    */
/****************************************************************************/

#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <stdio.h>
#include "strings.h"
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define LINELEN 256
#define COMMAND_LEN 456
#define MAXMODULES 14
#ifndef MAXPATHLEN
#define MAXPATHLEN  256
#endif

FILE *fp;
int masknumber;
char parameter_list[LINELEN];
char object_file_str[2*MAXPATHLEN];

struct modules {
		int flag;
		char *object_name;
		char *source_name;
		char *header_file;
		int module_masknumber;
	};

struct modules module_tbl[MAXMODULES];

int correct;  /* TRUE when the list of modules is syntactically correct */
int asgdir_exists = FALSE;
char stud_module_list[LINELEN];
char junk[LINELEN];
char source_lib_dir[MAXPATHLEN],      /* full path sources dir */
     object_lib_dir[MAXPATHLEN],
     assignment_dir[MAXPATHLEN],
     file_name[MAXPATHLEN]; 
char *all_object_files;
char *home_dir, *computer_architecture;
char command[COMMAND_LEN];
char *getenv(), *index();
char *c_compiler = "cc";

struct stat  buffer;

main(argc, argv)
    int argc;
    char *argv[];
{

   /* Initializations */


   all_object_files = "simcore.o memory.o cpu.o devices.o files.o resources.o sockets.o protocols.o dialog.o mask.o random.o inter.o devint.o pageint.o timeint.o procsvc.o iosvc.o";

   correct = FALSE;
  
   home_dir =  getenv("HOME");  /* get home directory */

   computer_architecture = index( argv[0], '.');
			/* get computer architecture gen is running on      */
			/* gen should have the name "gen.architecture-name" */
			/* index finds the last occurrence of '.'	    */

   sprintf(object_lib_dir,"%s/lib%s",home_dir,computer_architecture);
			/* construct the name of the library directory */

   if(stat(object_lib_dir, &buffer) != 0) {
         fprintf(stderr,"Can't find %s\n",object_lib_dir);
         exit(1);
    }

   gen_init();   

   printf("\f");
   printf("\n\n\n\n\n\n\n\n\n\n\n\n");
   printf("                     ******************************************\n");
   printf("                     *                                        *\n");
   printf("                     *          OSP Project Generator         *\n");
   printf("                     *               Release 1.7              *\n");
   printf("                     *                                        *\n");
   printf("                     *                                        *\n");
   printf("                     ******************************************\n");

   printf("\n\n\nCopyright (C) 1990  The Research Foundation of SUNY,\n");
   printf("\t\t    Michael Kifer and Scott S. Smolka\n");
   printf("\n(see $HOME/LICENSE for terms and conditions)\n");


   sprintf(source_lib_dir, "%s/src", home_dir);
			/* construct the name of the source library */

   if(stat(source_lib_dir, &buffer) != 0) {
         fprintf(stderr,"Can't find %s\n",source_lib_dir);
         exit(1);
   }  

   sleep(1);


   while (correct == FALSE)  {

   printf("\n\n\n\nModules: \n");
   printf("         a) MEMORY                  h) ALL INTERRUPTS:\n");
   printf("         b) CPU                         i) INTER      \n");
   printf("         c) DEVICES                     j) TIMEINT    \n");
   printf("         d) FILES                       k) PROCSVC    \n");
   printf("         e) RESOURCES                   l) PAGEINT    \n");
   printf("         f) SOCKETS                     m) DEVINT     \n");
   printf("         g) PROTOCOLS                   n) IOSVC      \n");

   printf("\n\nPls, enter selections corresponding to the modules\n");
   printf("to be implemented by the STUDENTS --> ");
   gets(stud_module_list);
   check_module_list(&correct);
   printf("\f");

  } /* end while correct = FALSE */

 
   printf("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
   printf("\n");
   mk_assignment_dir(assignment_dir);  /* assignment_dir initialized here */



/****************************************************************************/

   generate(stud_module_list,parameter_list,object_file_str,&masknumber);
				/* create most of the stuff */

   arrange_assignment_dir();  /* create hand_in & submissions dir inside
			         assignment dir    */


   /*  Take care of dialog.c and mask.[co], then exit  */

   sprintf(file_name,"%s/dialog.c",assignment_dir);
   if(stat(file_name, &buffer) != 0 || user_says_replace(file_name, "File")) {
       sprintf(command,"cat %s/dialog.c > %s/dialog.c",
					source_lib_dir, assignment_dir);
       system(command);
   }

   chdir(source_lib_dir);

   fp = fopen("mask.h","w");
   fprintf(fp,"#define MASK %d\n",masknumber);  /* make mask.h */
   fclose(fp);

   sprintf(command, "%s -c mask.c ; /bin/mv mask.o %s/mask.o",
			c_compiler, object_lib_dir);
   system(command);

   chdir(object_lib_dir);

   sprintf(command,"ld -r %s mask.o -o %s/osp.o ",
					object_file_str, assignment_dir);
   system(command);

   sprintf(command, "chmod 644 %s/Makefile %s/*.c %s/*.o", assignment_dir,
							   assignment_dir,
							   assignment_dir);
   system(command);

   sprintf(command, "%s %s -lm -o %s/OSP.demo",
				c_compiler, all_object_files, assignment_dir);
   system(command);

   sprintf(command, "chmod 711 %s/OSP.demo ", assignment_dir);
   system(command);

   sprintf(command, "chmod 755 %s ", assignment_dir);
   system(command);

   printf(" \n\n\n\t\t\t DONE! \n\n\n");
   exit(0);
}

/****************************************************************************/
/*                      function check_module_list()                        */
/* This checks the input to make sure it is within the correct range.       */
/****************************************************************************/
check_module_list(correct)
int   *correct;
{
   int all_interrupts_selected = FALSE,
       individual_interrupts_selected = FALSE;
   int len, ctr;
   char oneof_stud_modules;

   ctr = 0;
   len = strlen(stud_module_list);

   while ( ctr < len )  {
        switch ( oneof_stud_modules = stud_module_list[ctr++] ) {
          case 'a':
          case 'b':
          case 'c':
          case 'd':
          case 'e':
          case 'f':
          case 'g':
	     *correct = TRUE;
             break;

          case 'h':
	     *correct = TRUE;
             all_interrupts_selected = TRUE;
             if (individual_interrupts_selected == TRUE)  {
		*correct = FALSE;
		printf("\n\n\nWith 'h', selections 'i' to 'n' are redundant...\n");
	     }
             break; 

          case 'i':
          case 'j':
          case 'k':
          case 'l':
          case 'm':
          case 'n':
               *correct = TRUE;
               individual_interrupts_selected = TRUE;
               if (all_interrupts_selected == TRUE)  {
		   *correct = FALSE;
		    printf("\n\n\nWith 'h', selections 'i' to 'n' are redundant...\n");
	       }
               break;

	  case ' ':
	       continue;
          default:
               *correct = FALSE;
               goto incorrect;
        }  /* end switch */
       
        /* check for repetitions  in the input string*/

        if ( index( stud_module_list+ctr, oneof_stud_modules) != NULL )  {
               *correct = FALSE;
               printf("\n\nModule mentioned more than once...\n\n");
 	       break;
        }
          
    }  /* end while */

incorrect:
    if ( *correct == FALSE )  {   /* if an error was found */
      printf("\n\nSyntax Error. Hit <RETURN> to restart the generator... ");
      gets(junk);
    }
}




/****************************************************************************/
/*                      function  mk_assignment_dir()                       */
/*  			makes the assignment directory			    */
/****************************************************************************/
mk_assignment_dir(assignment_dir)
char  *assignment_dir;
{
     char answer;
     char scratchpad[LINELEN], dirname[MAXPATHLEN];

     
TRY_AGAIN:
     printf("Pls complete the name of the assignment directory:\n\t %s/", home_dir);

     gets(scratchpad);

     if ( ! ( isalnum(scratchpad[0]) )  )
     {
	printf("\n\n\n\ninvalid directory name ... try again\n\n");
	goto TRY_AGAIN;
     }

     sprintf ( dirname, "%s", scratchpad );

     if ( (strncmp(dirname, "lib", 3) == 0 ) ||
	  (strcmp(dirname, "bin") == 0 ) ||
	  (strcmp(dirname, "admin") == 0 ) ||
	  (strncmp(dirname, "src", 3) == 0 ) ||
	  (strcmp(dirname, "doc") == 0 ) ||
	  (strcmp(dirname, "gen") == 0 )    )
     {
	printf("\n\n'%s'  is a protected OSP directory ... try again\n\n",
				dirname);
        goto TRY_AGAIN;
     }

     sprintf (assignment_dir, "%s/%s%s", home_dir,
				dirname, computer_architecture);

     printf("\nThe assignment directory is %s\n", assignment_dir);

     if (stat(assignment_dir, &buffer) == 0)  {
			/* the file with this name exists */

	 if ( ! (buffer.st_mode & S_IFDIR))  {  /* but is not a directory */

	    printf("\nFile  %s  already exists, but is not a directory\n",
				assignment_dir);
	    printf("do you want to clobber this file (y/n)?  n ");
	    gets(scratchpad);
	    answer = scratchpad[0];

	    if ( answer == 'y' ) {
		sprintf(command, "/bin/rm -r -f  %s", assignment_dir);
		system(command);
						/* remove it */
		sprintf(command, "/bin/mkdir  %s", assignment_dir);
	        system(command);
						/* and create anew */
            } else
		exit(1);

         } else  {		/* and is a directory */

	    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	    printf("\n\t\t\t !!!!!!! \n");

	    printf("\nDirectory  %s  already exists\n", assignment_dir);
	    printf("\nFiles 'osp.o' and 'OSP.demo' will be written over\n\n");
	    printf("Pls advise regarding the disposition of 'Makefile' and 'module.c' files\n");

	    printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

            asgdir_exists = TRUE;
	    
	 }

          sprintf(command, "chmod 755  %s", assignment_dir );
          system(command);
      }  else { 		/* no file with this name */
	  sprintf(command, "/bin/mkdir  %s", assignment_dir );
	  system(command);
					/* create the directory */
      }


}


/**************************************************************************/
/*                          Function arrange_assignment_dir()             */
/* This function makes a 'hand_in' shell script and the submissions 	  */
/* subdirectory inside the assignment directory 			  */
/*									  */
/**************************************************************************/
arrange_assignment_dir()
{  
FILE *hand_in;

	if (chdir(assignment_dir) != 0)  {
		fprintf(stderr, "Can't chdir to  %s\n", assignment_dir);
		exit(1);
	}

	if (stat("submissions",&buffer) != 0 
			|| user_says_replace("submissions", "Directory")){
	    system( "/bin/rm -r -f  submissions" );
	    system( "/bin/mkdir  submissions" );
	}

	/* create the hand_in file */

	system( "/bin/rm -f hand_in" );
	if ( NULL == (hand_in = fopen("hand_in", "w") ) )  {
                fprintf(stderr,
			"gen: can't create the 'hand_in' shell script\n");
                exit(1);
        }

	fprintf(hand_in, "%s/bin/run_osp%s  %s '%s'  \n", home_dir,
					computer_architecture,
					assignment_dir,
					parameter_list);
	fclose ( hand_in );

	system("chmod 555 hand_in");
	system("chmod 755 submissions");
}

/****************************************************************************/
gen_init()
{

    int i;

    for (i = 0;i<MAXMODULES;i++)
       module_tbl[i].flag = FALSE;

	    /* init module table */

    module_tbl[0].object_name = "memory.o ";
    module_tbl[0].source_name = "memory.c ";
    module_tbl[0].header_file = "memory.h ";
    module_tbl[0].module_masknumber = 1;

    module_tbl[1].source_name = "cpu.c ";
    module_tbl[1].object_name = "cpu.o ";
    module_tbl[1].header_file = "cpu.h ";
    module_tbl[1].module_masknumber = 2;

    module_tbl[2].object_name = "devices.o ";
    module_tbl[2].source_name = "devices.c ";
    module_tbl[2].header_file = "devices.h ";
    module_tbl[2].module_masknumber = 4;

    module_tbl[3].object_name = "files.o ";
    module_tbl[3].source_name = "files.c ";
    module_tbl[3].header_file = "files.h ";
    module_tbl[3].module_masknumber = 8;

    module_tbl[4].object_name = "resources.o ";
    module_tbl[4].source_name = "resources.c ";
    module_tbl[4].header_file = "resources.h ";
    module_tbl[4].module_masknumber = 16;

    module_tbl[5].object_name = "sockets.o ";
    module_tbl[5].source_name = "sockets.c ";
    module_tbl[5].header_file = "sockets.h ";
    module_tbl[5].module_masknumber = 32;

    module_tbl[6].object_name = "protocols.o ";
    module_tbl[6].source_name = "protocols.c ";
    module_tbl[6].header_file = "protocols.h ";
    module_tbl[6].module_masknumber = 64;

    module_tbl[7].object_name = "";
    module_tbl[7].source_name = "";
    module_tbl[7].header_file = "";
    module_tbl[7].module_masknumber = 128;    /* place holder for all of OSP */

    module_tbl[8].object_name = "inter.o ";
    module_tbl[8].source_name = "inter.c ";
    module_tbl[8].header_file = "inter.h ";
    module_tbl[8].module_masknumber = 256;

    module_tbl[9].object_name = "timeint.o ";
    module_tbl[9].source_name = "timeint.c ";
    module_tbl[9].header_file = "timeint.h ";
    module_tbl[9].module_masknumber = 512;

    module_tbl[10].object_name = "procsvc.o ";
    module_tbl[10].source_name = "procsvc.c ";
    module_tbl[10].header_file = "procsvc.h ";
    module_tbl[10].module_masknumber = 1024;

    module_tbl[11].object_name = "pageint.o ";
    module_tbl[11].source_name = "pageint.c ";
    module_tbl[11].header_file = "pageint.h ";
    module_tbl[11].module_masknumber = 2048;

    module_tbl[12].object_name = "devint.o ";
    module_tbl[12].source_name = "devint.c ";
    module_tbl[12].header_file = "devint.h ";
    module_tbl[12].module_masknumber = 4096;

    module_tbl[13].object_name = "iosvc.o ";
    module_tbl[13].source_name = "iosvc.c ";
    module_tbl[13].header_file = "iosvc.h ";
    module_tbl[13].module_masknumber = 8192;
}
/***************************************************************************/


generate(stud_module_list,parameter_list,object_file_str,masknumber)
char *stud_module_list,*parameter_list,*object_file_str;
int *masknumber;
{
    int i,j,selection;
    char *aux_ptr;
    char temp_osp[3*LINELEN],temp_list[3*LINELEN];
    char makefile_line[LINELEN];
    char copy_of_makefile_line[LINELEN],command2[LINELEN];    
    
    i = 0;

    while(i < strlen(stud_module_list))
    {
	    selection = (int)(stud_module_list[i]);
	    if (! (isspace(stud_module_list[i])) )
                                /*  if a non alpha is read  skip over */
	    {
               selection = selection - (int) 'a';
 	       if (selection == 7) 
	       {
	 	  for(j = 8;j<MAXMODULES;j++)
		       module_tbl[j].flag = TRUE;
               }
	       else
		    module_tbl[selection].flag = TRUE;
		   
	    }
	    i = i + 1;
    }

    sprintf(object_file_str,"");
    sprintf(parameter_list,"");
    sprintf(command2,"%s/.Makefile",assignment_dir);
    fp = fopen(command2,"w");

   fputs("###############################################################\n",fp);
   fputs("#   Students' Makefile for Operating Systems Project          #\n",fp);
   fputs("#                     Use As Is                               #\n",fp);
   fputs("###############################################################\n",fp);


    fprintf(fp, "\n\n# The assignment directory \n");
    fprintf(fp, "ASG_DIR = %s/\n", assignment_dir);

    fprintf(fp, "\n# The default C compiler \n");
    fprintf(fp, "C_COMP = %s \n", c_compiler);

    fprintf(fp, "\n\n#make OSP:  \n\n",fp);
      
    i = 0;
    sprintf(makefile_line,""); 
    sprintf(temp_list,"");
    while (i < MAXMODULES)
    {
      sprintf(makefile_line,"%s",temp_list);
      if (module_tbl[i].flag == TRUE)
        sprintf(temp_list,"%s%s",makefile_line,module_tbl[i].object_name);  
      i++;
    }

    fprintf(fp,"OSP : $(ASG_DIR)osp.o dialog.o %s\n",temp_list);
    fprintf(fp,"\t$(C_COMP) $(ASG_DIR)osp.o dialog.o %s %s\n\n",
                                temp_list,"-lm -g -o OSP");

    fprintf(fp,"dialog.o : dialog.c\n");
    fprintf(fp,"\t$(C_COMP) -c -g dialog.c\n\n");

    i = 0;
    *masknumber = 0;
    while(i < MAXMODULES)
    {  

	if( strcmp(module_tbl[i].source_name, "") == 0 ) {
		i++;
		continue;  /* continue if module is not yet implemented */
  	}

        sprintf(temp_osp,"%s",object_file_str);
	sprintf(temp_list,"%s",parameter_list);
	sprintf(copy_of_makefile_line,"%s",makefile_line);

	if (module_tbl[i].flag == TRUE) {  /* student will implement these */

	  *masknumber = *masknumber + module_tbl[i].module_masknumber;
	   sprintf(parameter_list,"%s%s", temp_list,module_tbl[i].source_name);
	   sprintf(makefile_line,"%s%s", copy_of_makefile_line,
					 module_tbl[i].object_name);

	   sprintf(file_name,"%s/%s",assignment_dir,module_tbl[i].source_name);
	   aux_ptr = index(file_name, ' ');
	   *aux_ptr = '\0';   /* delete trailing space that 'stat' dislikes */

	   if(stat(file_name, &buffer) != 0 
				|| user_says_replace(file_name,"File")) {
	       sprintf(command2,"cat %s/src/%s %s/gen/%s > %s",
			  home_dir, module_tbl[i].header_file,
			  home_dir,module_tbl[i].source_name,
			  file_name);
	       system(command2);
	   }
			  
	   fprintf(fp,"%s: %s\n",module_tbl[i].object_name,
				module_tbl[i].source_name);
	   fprintf(fp,"\t$(C_COMP) -c -g %s\n\n",module_tbl[i].source_name);
	

        } /* end if  TRUE */
        else { /* gather modules which will be in osp.o */

            sprintf(object_file_str,"%s%s",temp_osp,module_tbl[i].object_name);
      }

      i++;

   } /* end while */
   fclose(fp);

   sprintf(file_name,"%s/Makefile",assignment_dir);
   if (stat(file_name, &buffer) != 0 || user_says_replace(file_name,"File") ) {
			    /* No Makefile or Makefile exists, but replace */
	   sprintf(command2,"mv %s/.Makefile %s/Makefile",
			  assignment_dir, assignment_dir);
	   system(command2);
   }

	

	

    /* create list of object file names to be returned */
   sprintf(temp_osp,"%s",object_file_str);
   sprintf(object_file_str,"%ssimcore.o random.o ", temp_osp);

}

user_says_replace(file_name, type)
char *file_name, *type;
{
   char scratchpad[LINELEN];
   char answer;

	printf("\n%s '%s' exists.\n\t Replace?  (y/n) n ", type, file_name);

	gets(scratchpad);
	answer = scratchpad[0];

	if ( answer == 'y' )  {
		printf("\n\t %s replaced\n\n", type);
		return(TRUE);
	}
	else {
		printf("\n\t %s preserved\n\n", type);
		return(FALSE);
	}
}
