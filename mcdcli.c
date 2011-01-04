/*
 * mcdcli.c - Command-line driver for music CD database.  Suitable
 * for use in shell scripts.
 */
#include <stdlib.h>		/* For `exit' */
#include <unistd.h>
#include <getopt.h>		/* For `getopt' */
#include <string.h>		/* For `memcpy' in glibc 2.1.1 */
#include "mcddb.h"		/* Database management */

#define BUFSZ 1024

void usage(void);

int main(int argc, char **argv)
{
     int ret, op, cnt, i;
     extern char *optarg;	/* From <getopt.h> */
     extern int optind;		/* From <getoph.h> */
     DBT value;
     char **keys, **values;

     if(argc < 2 || argc > 4) {
	 usage();
     }

     op = getopt(argc, argv, "a:d:f:g:l");

     if(open_db(NULL) == 1) {		/* Open the database */
	 puts("Error opening database");
     }

     switch(op) {
     case 'a':			/* Add a record */
	 /* But don't add an empty record or a zero-length key */
	 if(argc == 4 &&
	    (optarg != NULL) && 
	    (strlen(optarg) >= 1) &&
	    (argv[optind] != NULL)) {
	     ret = add_rec(optarg, argv[optind]);
	     if(ret == 1) {
		 printf("Key `%s' exists\n", optarg);
		 exit(EXIT_FAILURE);
	     } else if (ret < 0) {
		 perror("mcdcli.c: add_rec");
		 exit(EXIT_FAILURE);
	     }
	     break;
	 } else {
	     usage();
	 }
     case 'd':			/* Delete a record */
	 if(argc == 3) {
	     if(optarg != NULL) {
		 ret = del_rec(optarg);
		 if(ret == 1) {
		     printf("Key `%s' not found\n", optarg);
		     exit(EXIT_FAILURE);
		 } else if(ret < 0) {
		     perror("mcdcli.c: del_rec");
		     exit(EXIT_FAILURE);
		 }
		 break;
	     } 
	 } else {
	     usage();
	 }
     case 'f':			/* Find a record */
	 if(argc == 3) {
	     if(optarg != NULL) {
		 memset(&value, 0, sizeof(DBT));
		 ret = find_rec(optarg, &value);
		 if(ret == 1) {
		     printf("Key `%s' not found\n", optarg);
		     exit(EXIT_FAILURE);
		 } else if(ret < 0) {
		     perror("mcdcli.c: find_rec");
		     exit(EXIT_FAILURE);
		 }
		 printf("%s|%.*s\n", optarg,
			(int)value.size, (char *)value.data);
		 break;
	     }
	 } else {
	     usage();
	 }
     case 'g':			/* Get a record */
	 if(argc == 3) {
	     if(optarg != NULL) {
		 memset(&value, 0, sizeof(DBT));
		 ret = get_rec(optarg, &value);
		 if(ret == 1) {
		     printf("Key `%s' not found\n", optarg);
		     exit(EXIT_FAILURE);
		 } else if(ret < 0) {
		     perror("mcdcli.c: get_rec");
		     exit(EXIT_FAILURE);
		 }
		 printf("%.*s|%.*s\n", (int)strlen(optarg), optarg,
			(int)value.size, (char *)value.data);
		 break;
	     } 
	 } else {
	     usage();
	 }
     case 'l':			/* List all records */
	 if(argc == 2) {
	     if((cnt = count_recs()) == 0) {
		 puts("No records in database");
		 exit(EXIT_FAILURE);
	     }
	     /* Size the pointer pointers */
	     if((keys = malloc(sizeof(DBT *) * cnt)) == NULL) {
		 puts("mcdcli.c: malloc keys");
		 exit(EXIT_FAILURE);
	     }
	     if((values = malloc(sizeof(DBT *) * cnt)) == NULL) {
		 puts("mcdcli.c: malloc values");
		 exit(EXIT_FAILURE);
	     }
	     /* Size each element */
	     for(i = 0; i < cnt; ++i) {
		 if((keys[i] = malloc(BUFSZ)) == NULL) {
		     puts("mcdcli.c: malloc keys[i]");
		     exit(EXIT_FAILURE);
		 }
		 if((values[i] = malloc(BUFSZ)) == NULL) {
		     puts("mcdcli.c: malloc values[i]");
		     exit(EXIT_FAILURE);
		 }
	     }
	     ret = list_recs(keys, values);
	     if(ret == 0) {
		 perror("mcdcli.c: list_recs");
		 exit(EXIT_FAILURE);
	     }
	     for(i = 0; i < cnt; ++i) {
		 printf("%.*s|%.*s\n", 
			(int)strlen(keys[i]), keys[i],
			(int)strlen(values[i]), values[i]);
	     }
	     break;
	 } else {
	     usage();
	 }
     default:			/* Bad keystroke, show the menu */
	 usage();
	 break;
     } /* switch */
		
     close_db();		/* Close the database */
     exit(EXIT_SUCCESS);
}

void usage(void)
{
    puts("USAGE: mcdcli \
	 \n\t{-a <key> <value>}\
	 \n\t{-d <key>}\
	 \n\t{-f <key>}\
	 \n\t{-g <key>}\
	 \n\t{-l}");
    exit(EXIT_FAILURE);
}
