/*  Time Delay and custom Window creation.  */

#include <ncurses.h>
#include <stdio.h>
#include <time.h>

int waitatick(void);

int main(void)
{
	WINDOW *win1;
	int y, x;

	initscr();
	cbreak();		// Line buffering disabled
	keypad(stdscr, TRUE);	// allow softkeys
	noecho();		// don't display input when getch();
	getmaxyx(stdscr,y,x);

	win1 = newwin(y,x,0,0);
	mvwprintw(win1, y/2-2, 0, "This is printed in window 1.");
	wrefresh(win1);

	waitatick();
	mvwprintw(win1, y/2+2, 0, "Fuck yeahhhh.");
	wrefresh(win1);

	waitatick();
	waitatick();
	delwin(win1);
	endwin();
	return 0;
}



int waitatick(void)
{
	struct timespec delay;

	delay.tv_sec = 0;
	delay.tv_nsec = 500000000L;

	if (nanosleep(&delay,NULL)) {
		perror("nanosleep");
		return 1;
	}

	return 0;
}
