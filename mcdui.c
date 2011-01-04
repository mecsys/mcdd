/*
 * mcdui.c - Ncurses-based driver for music CD database.
 */
#include <stdlib.h>		/* For `exit' */
#include <unistd.h>
#include <getopt.h>		/* For `getopt' */
#include <string.h>		/* For `memcpy' in glibc 2.1.1 */
#include "mcddb.h"		/* Database management */
#include "mcdscr.h"		/* Screen handling */

#define BUFSZ 1024

void usage(void);

int main(int argc, char **argv)
{
    int ret, opt, fkey, cnt, i;
    extern char *optarg;	/* From <getopt.h> */
    char kbuf[BUFSZ], vbuf[BUFSZ];
    char **keys, **values;
    DBT value;

    /* Parse the command line */
    switch(argc) {
    case 3:			/* Use the specified database */
	opt = getopt(argc, argv, "f:");
	if(opt == 'f') {
	    if(optarg == NULL) {
		usage();
	    } else {
		if(open_db(optarg)) {
		    fprintf(stderr, "Error opening database %s\n", optarg);
		    exit(EXIT_FAILURE);
		}
	    }
	}
	break;
    case 1:			/* Use the default database */
	if(open_db(NULL)) {
	    puts("Error opening database");
	    exit(EXIT_FAILURE);
	}
	break;
    default:			/* Malformed command-line */
	usage();
	break;
    } /* end switch(opt) */
	
	
    /* Start ncurses */
    if(init_scrn() < 0) {	/* Bummer, ncurses didn't start */
	puts("Error initializing ncurses");
	close_db();
	exit(EXIT_FAILURE);
    }
    /* Draw the initial screen */
    draw_scrn();

    /* The primary command loop */
    while((fkey = getch()) != KEY_F(10)) {
	switch(fkey) {
	case KEY_F(2) :		/* F2 -Add a record */
	    add(kbuf, vbuf);
	    ret = add_rec(kbuf, vbuf);
	    if(ret > 0) 
		status("Key already exists");
	    else if(ret < 0)
		status("Unknown error occurred");
	    else
		status("Record added");
	    break;
	case KEY_F(3) :		/* F3 - Delete a record */
	    del(kbuf);
	    ret = del_rec(kbuf);
	    if(ret > 0)
		status("Key not found");
	    else if(ret < 0)
		status("Unknown error occurred");
	    else
		status("Record deleted");
	    break;
	case KEY_F(4) :		/* F4 - Find a record */
	    find(kbuf);
	    memset(&value, 0, sizeof(DBT));
	    ret = find_rec(kbuf, &value); 
	    if(ret > 0)
		status("Key not found");
	    else if(ret < 0)
		status("Unknown error occurred");
	    else {
		status("Record located");
		sprintf(vbuf, "%.*s", 
			(int)value.size, (char *)value.data);
		show(kbuf, vbuf);
	    }
	    break;
	case KEY_F(5) :		/* F5 - Get a record */
	    get(kbuf);
	    memset(&value, 0, sizeof(DBT));
	    ret = get_rec(kbuf, &value);
	    if(ret > 0)
		status("Key not found");
	    else if(ret < 0)
		status("Unknown error occurred");
	    else
		status("Record located");
	    sprintf(vbuf, "%.*s",
		    (int)value.size, (char *)value.data);
	    show(kbuf, vbuf);
	    break;
	case KEY_F(6):		/* F6 - List all records */
	    if((cnt = count_recs()) == 0) {
		status("No records in database");
		break;
	    } 
	    /* Size a table of pointers */
	    if((keys = malloc(sizeof(DBT *) * cnt)) == NULL)
		status("Memory error");
	    if((values = malloc(sizeof(DBT *) * cnt)) == NULL)
		status("Memory error");
	    /* Size each pointer in the table */
	    for(i = 0; i < cnt; ++i) {
		if((keys[i] = malloc(BUFSZ)) == NULL) {
		    status("Memory error");
		    break;
		}
		if((values[i] = malloc(BUFSZ)) == NULL) {
		    status("Memory error");
		    break;
		}
	    }
	    /* Get all of the records */
	    ret = list_recs(keys, values);
	    if(ret == 0) {
		status("Problem with database manager");
		break;
	    }
	    /* Show 'em on the screen */
	    for(i = 0; i < cnt; ++i) {
		sprintf(kbuf, "%.*s", (int)strlen(keys[i]), keys[i]);
		sprintf(vbuf, "%.*s", (int)strlen(values[i]), values[i]);
		list(kbuf, vbuf, i);
	    }
	    status("Last record displayed");
	    break;
	default:		/* Bad keystroke, nag user */
	    status("Key not defined");
	    break;
	} /* End switch(fkey) */
    }
    
    close_db();			/* Close the database */
    close_scrn();		/* Close curses subsystem */
    exit(EXIT_SUCCESS);
}

/*
 * Simple usage nag
 */
void usage(void)
{
    puts("USAGE: mcdui [-f database]");
    exit(EXIT_FAILURE);
}
