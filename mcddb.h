/*
 * mcddb.h - Database module for music CD database
 */
#ifndef MCDDB_H_
#define MCDDB_H_

#include <db.h>

/*
 * Open the database named in dbpath, or, if it is NULL, assume that
 * we want PWD/mcd.db. Return 0 on success, errno or -1 on failure.
 */
int open_db(char *dbpath);

/*
 * Sync and close the currently open database. Always returns 0.
 */
int close_db(void);

/*
 * Add the record with a key of kbuf and a value of vbuf to the
 * currently opened database. Returns 0 on success, 1 if the key
 * already exists, and errno otherwise.
 */
int add_rec(char *kbuf, char *vbuf);

/*
 * Delete the record whose key matches kbuf. On success, syncs the 
 * database to disk and returns 0; on failure, returns 1 if a key
 * matching kbuf was not found or errno otherwise.
 */
int del_rec(char *kbuf);

/*
 * Iterate through the database, searching for a key matching kbuf.
 * On success, returns 0 and stores the data corresponding to kbuf
 * in value. On failure, returns DB_NOUTFOUND or errno.
 */
int find_rec(char *kbuf, DBT *value);

/*
 * Retrieve the record matching key from the database. Returns 0 on
 * success and stores the corresponding value in value, 1 if the 
 * indicated key is not found, and errno otherwise.
 */
int get_rec(char *kbuf, DBT *value);

/*
 * Count the number of records in the database by iterating through it
 * with a cursor. Returns the number of records if successful or 0 if
 * the database is empty or an error occurs.
 */
int count_recs(void);

/*
 * Iterate through the entire database, listing each record consecutively
 * and alphabetically by key. Returns number of records stored in kbuf and
 * vbuf on success. Returns 0 if no records available. On failure, returns 
 * errno.
 */
int list_recs(char **keys, char **values);
#endif /* MCDDB_H_ */
