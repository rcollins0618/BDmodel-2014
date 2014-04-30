/*  This program checks for the terminal size and updates the user as to
whether it is large enough or not upon pressing any normal key (no soft keys).  */

#include <string.h>
#include <ncurses.h>

void initscreen(void);


int main(void)
{
	int row = 0, col = 0;

	initscreen();

	clear();
	system("clear");
	return 0;
}



void initscreen(void) {
	char msg0[]="For this program, the screen must be at least 100 x 35 characters.";
	char msg1[]="Please increase the size of the screen.";
	char msg2[]="Press any key when complete.";
	int row = 0, col = 0;

	initscr();
	getmaxyx(stdscr, row, col);

	while(col<100 || row<35)
	{
		clear();
		getmaxyx(stdscr, row, col);
		mvprintw(row/2-1,(col-strlen(msg0))/2,"%s",msg0);
		mvprintw(row/2-5,col/2-15,"Current Window Size: %d x %d",col,row);
		mvprintw(row/2+1,(col-strlen(msg1))/2,"%s",msg1);
		mvprintw(row/2+2,(col-strlen(msg2))/2,"%s",msg2);
		refresh();
		getch();
	}
}
