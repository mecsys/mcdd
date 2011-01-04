/*
 * mcdscr.c - Screen handling for music CD database
 */
#include <curses.h>
#include <form.h>
#include "mcdscr.h"

WINDOW *mainwin;		/* Primary window */
WINDOW *menuwin;		/* Menu bar at top of mainwin */
WINDOW *statuswin;		/* Status line at bottom of mainwin */
WINDOW *workwin;		/* Input/output area of mainwin */

int init_scrn(void)
{
    int maxy, maxx;

    if((mainwin = initscr()) == NULL) {
	perror("mcdscr.c: mainwin");
	return -1;
    }
    getmaxyx(stdscr, maxy, maxx);

    /* Subwindow to write a "menu" on */
    if((menuwin = derwin(stdscr, 1, maxx, 0, 0)) == NULL) {
	perror("mcdscr.c: menuwin");
	return -1;
    }
    /* Subwindow to write status messages on */
    if((statuswin = derwin(stdscr, 1, maxx, maxy - 1, 0)) == NULL) {
	perror("mcdscr.c: statuswin");
	return -1;
    }
    /* Subwindow where input and output occurs */
    if((workwin = derwin(stdscr, maxy - 2, maxx, 1, 0)) == NULL) {
	perror("mcdscr.c: workwin");
	return - 1;
    }
    /* Set up the keyboard */
    if(cbreak() == ERR)		/* Process input ourselves */
	return -1;
    if(keypad(stdscr, TRUE) == ERR) /* Enable use of F-keys */
	return -1;
    if(noecho() == ERR)		/* Control output ourselves */
	return -1;
    
    return 0;
}

void close_scrn(void)
{
    nocbreak();		/* Restore cooked mode */
    delwin(menuwin);		/* Free subwindws */
    delwin(statuswin);
    delwin(workwin);
    endwin();			/* Restore terminal state */
}       

void draw_scrn(void)
{
    char *menu = "F2-Add   F3-Delete   F4-Find   F5-Get   F6-List   F10-Quit";

    mvwprintw(menuwin, 0, 0, "%s", menu);
    wrefresh(menuwin);
    refresh();
}

void status(char *msg)
{
    werase(statuswin);
    mvwprintw(statuswin, 0, 0, "%s", msg);
    wrefresh(statuswin);
    refresh();
}

void prep_win(WINDOW *window)
{
    werase(window);
    echo();
}

void show_win(WINDOW *window)
{
    noecho();
    wrefresh(window);
    refresh();
}

void add(char *kbuf, char *vbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1,0, "ARTIST : ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    mvwprintw(workwin, 2, 0, "TITLE  : ");
    status("Enter value");
    wgetstr(workwin, vbuf);
    show_win(workwin);
}

void del(char *kbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST : ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    show_win(workwin);
}

void find(char *kbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST : ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    show_win(workwin);
}

void show(char *kbuf, char *vbuf)
{
    werase(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST : %s", kbuf);
    mvwprintw(workwin, 2, 0, "TITLE  : %s", vbuf);
    show_win(workwin);
}

void list(char *kbuf, char *vbuf, int cnt)
{
    int maxx, maxy, nexty;

    getmaxyx(workwin, maxy, maxx);
    if(cnt == 0) {
	werase(workwin);
	mvwhline(workwin, cnt, 0, ACS_HLINE, maxx);
	mvwprintw(workwin, cnt + 1, 0, "ARTIST");
	mvwprintw(workwin, cnt + 1, maxx / 2, "TITLE");
	mvwhline(workwin, cnt + 2, 0, ACS_HLINE, maxx);
    }
    nexty = cnt + 3;
    mvwprintw(workwin, nexty, 0, "%s", kbuf);
    mvwprintw(workwin, nexty, maxx / 2, "%s", vbuf);
    show_win(workwin);
}	

void get(char *kbuf)
{
    prep_win(workwin);
    mvwprintw(workwin, 1, 0, "ARTIST : ");
    status("Enter key");
    wgetstr(workwin, kbuf);
    show_win(workwin);
}
