/*
 * mcddb.c - Database manager for music CD database
 */
#include <db.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "mcddb.h"

static DB *db;			/* Global database pointer */

int open_db(char *dbpath)
{
    int ret;
    u_int32_t flags;
    char *defpath = "./mcd.db";

    /* If dbpath is NULL, assume the current dir name the DB mcd.db */
    if(!dbpath) {
	if((dbpath = malloc(strlen(defpath) + 1)) != NULL) {
	     strcpy(dbpath, defpath);
	} else {
	     return -1;
	}
    }

    ret = db_create(&db, NULL, 0);
    if(ret != 0)
	return ret; /* Fatal */
    
    flags = DB_CREATE;

    ret = db->open(db, NULL, dbpath, NULL, DB_BTREE, flags, 0);
    if(ret != 0)
        return ret; /* Fatal */

    return 0;
}

int close_db(void)
{
    /* db->close should force a sync, but just in case */
    db->sync(db, 0);
    db->close(db, 0);
    return 0;
}

int add_rec(char *kbuf, char *vbuf)
{
    DBT key, value;
    int ret;

    /* Initialize the DBTs */
    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));
     
    /* This looks perverse, but it works */
    key.data =  kbuf;
    key.size = strlen(kbuf);
    value.data =  vbuf;
    value.size = strlen(vbuf);

    /* Store the record */
    ret = db->put(db, NULL, &key, &value, DB_NOOVERWRITE);
    if(ret == DB_KEYEXIST) /* Key already exists */
	return 1;
    else if(ret != 0) /* Some other error occurred */
	return ret;
    /* Sync the database */
    db->sync(db, 0);
    return 0; /* Success! */
}

int del_rec(char *kbuf)
{
    int ret;
    DBT key;
     
    memset(&key, 0, sizeof(DBT));
    key.data =  kbuf;
    key.size = strlen(kbuf);

    ret = db->del(db, NULL, &key, 0);
    if(ret != 0) {
	if(ret == DB_NOTFOUND) { /* Key not found */
	    return 1;
	} else {
	    return ret;
	}
    }
    /* Sync the database */
    db->sync(db, 0);
    return 0;
}

int find_rec(char *kbuf, DBT *value)
{
    int ret;
    DBT key;			/* Copy kbuf here */
    DBC *dbc = NULL;		/* A database cursor */

    /* Create the cursor */
    ret = db->cursor(db, NULL, &dbc, 0);
    if(ret != 0) {			/* Something went wrong */
	return ret;
    }
    
    /* Initialize DBT key; assume `value' is already init'ed */
    memset(&key, 0, sizeof(DBT));
    key.data =  kbuf;
    key.size = strlen(kbuf);

    /* Iterate through the database, looking for a match */
    while((ret = dbc->c_get(dbc, &key, value, DB_NEXT)) != DB_NOTFOUND) {
	/*
	 * This is tricky. db does not store null-terminated strings,
	 * so use strncmp to limit the bytes compared to the length
	 * of the search string.  To make sure the match is legitimate,
	 * compare the length of the search string to key.size. If they
	 * are equal, assume the match is good.
	 */
	if(!strncmp(key.data, kbuf, strlen(kbuf))) {
	    if(key.size == strlen(kbuf)){ /* Found a match */
		break;
	    }
	}
    }
    /* Did we fall through or find a match? */
    if(ret == DB_NOTFOUND) { 
	return 1;
    }
    return 0;			/* Happy, happy, joy, joy */
}

int get_rec(char *kbuf, DBT *value)
{
    int ret;
    DBT key;

    /* Initialize the key DBT; assume value init'ed by caller */
    memset(&key, 0, sizeof(DBT));
    key.data = kbuf;
    key.size = strlen(kbuf);

    /* See if we can get the record */
    ret = db->get(db, NULL, &key, value, 0);
    switch(ret) {
    case 0:			/* Bliss */
	return 0;
    case DB_NOTFOUND:		/* Key not found */
	return 1;
    default:			/* Unknown error */
	return ret;		
    } /* end switch */
}

int count_recs(void) {
    int ret, cnt = 0;
    DBT key, value;
    DBC *dbc = NULL;
    
    /* Create the cursor */
    ret = db->cursor(db, NULL, &dbc, 0);
    if(ret != 0)
	return ret;

    /* Initialize the DBTs */
    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));
    
    while((dbc->c_get(dbc, &key, &value, DB_NEXT)) != DB_NOTFOUND) {
	++cnt;
    }
    return cnt;
}

int list_recs(char **keys, char **values)
{
    int ret, cnt = 0;
    DBT key, value;
    DBC *dbc = NULL;		/* Database cursor */

    /* Create the cursor */
    ret = db->cursor(db, NULL, &dbc, 0);
    if(ret != 0) {
	fprintf(stderr, "mcddb.c: db->cursor: %s\n", strerror(ret));
	return 0;
    }

    /* Initialize the DBTs */
    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));
    
    /* Move sequentially through the database */
    while((dbc->c_get(dbc, &key, &value, DB_NEXT)) != DB_NOTFOUND) {
	
        if(db->get(db, NULL, &key, &value, 0))
            perror("DB->get");
	memcpy(keys[cnt], key.data, key.size);
	memcpy(values[cnt], value.data, value.size);
      	++cnt;
    }

    return cnt;
}
