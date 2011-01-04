/*
 * mcdscr.h - Screen-handling for music CD database
 */
#ifndef MCDSCR_H_		/* Guard against multiple inclusion */
#define MCDSCR_H_

#include <curses.h>

/*
 * Initialize the curses subsystem. Return 0 if successful or -1
 * on error.
 */
int init_scrn(void);

/*
 * Shut down the curses sybsystem. No return value.
 */
void close_scrn(void);

/*
 * (Re)Draw the main screen. No return value.
 */
void draw_scrn(void);

/*
 * Print a message to the status line.
 */
void status(char *msg);

/*
 * Prepare a window for input and output.
 */
void prep_win(WINDOW *win);

/*
 * Prepare a window to be redisplayed.
 */
void show_win(WINDOW *win);

/*
 * Get a key/value pair to add to the database.
 */
void add(char *kbuf, char *vbuf);

/*
 * Get the key of a record to delete.
 */
void del(char *kbuf);

/*
 * Get the key of a record to find.
 */
void find(char *kbuf);

/*
 * Display a key/value pair
 */
void show(char *kbuf, char *vbuf);

/*
 * Get a specifed key/value pair
 */
void get(char *kbuf);

/*
 * List all records real purty-like
 */
void list(char *kbuf, char *vbuf, int cnt);

#endif /* MCDSCR_H_ */
