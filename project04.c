/*  Press the ANY Key to display a second magical screen!  */

#include <ncurses.h>
#include <stdio.h>
#include <time.h>

void waitms(int);
void waitmics(int);

int main(void)
{
        WINDOW *win1;
        int y, x, i, j, k;	// j, k not used yet.
	int xidx;
	float xmu[3500] = {}, xvar[3500] = {}, deltamu[3500] = {}, deltavar[3500] = {};
	long xsum[3500];
	int xrnd[3500] = {};
	int xused[3500] = {};
	int scnarr[35][100] = {};
	int slowness = 50;

        initscr();
        cbreak();               // Line buffering disabled
        keypad(stdscr, TRUE);   // allow softkeys
        noecho();               // don't display input when (w)getch();
	curs_set(0);		// don't display the cursor.
        getmaxyx(stdscr,y,x);

	srand((unsigned)(int)time(NULL));

        win1 = newwin(y,x,0,0);

	for (i=0; i<3500; i++) {	// i == xrnd[index]
		xrnd[i] = rand()%100;
	}				// end i.
	for (i=0; i<3500; i++) {				// i == xrnd[] index
		xidx = i;
		if (scnarr[0][xrnd[i]] == 1)
			break;
		for (j=34; j>=0; j--) {				// j == vertical scanner
			if (scnarr[j][xrnd[i]] != 1) {
				scnarr[j][xrnd[i]] = 1;
				xused[i] = xrnd[i];
				for (k=0; k<=j; k++) {		// k == white drop spaace
					if (k>0)
						mvwprintw(win1,k-1,xrnd[i]," ");
					mvwprintw(win1,k,xrnd[i],"O");
					wrefresh(win1);
 // Enable This Line on LINUX hosts:	waitmics(slowness);
				}
				break;
			}
		}
	}
	wgetch(win1);
	wclear(win1);
	xidx--;
	for (i=0; i<xidx; i++) {
		if (i == 0)	xsum[i]=xrnd[i];
		else		xsum[i] = xsum[i-1] + xrnd[i];
		xmu[i] = (float)xsum[i]/(i+1);
		if (i == 0)	deltamu[0] = xmu[0];
		else		deltamu[i] = xmu[i]-xmu[i-1];
	}

/**_debug_and_test_code_**********************************************************************
 *
 *		STUFF GOES HERE - also, useful divider at 95 cols in length, below:
**********************************************************************************************
 *
 */
//	mvwprintw(win1,0,10,"            \t                     ");
	mvwprintw(win1,1,10," xmu = %.3f\tdeltamu[%d]= %.4f ",xmu[xidx-1],xidx-1,deltamu[xidx-1]);
//	mvwprintw(win1,2,10,"            \t                    ");
	mvwprintw(win1,2,10," xmu[0:15]: ");
	for (i=0; i<15; i++) {
		mvwprintw(win1, 35-((int)xmu[i]*35/100), i*6, "%.2f", xmu[i]);
		mvwprintw(win1, 35-((int)xmu[i]*35/100)+1, i*6, "%.2f", deltamu[i]);
	}
	wrefresh(win1);
	waitms(3000);
	while (1) {
		k = wgetch(win1);
		if (k != 10) {
			mvwprintw(win1, 0, 0, "XXX");
			wrefresh(win1);
		} else if (k == 10) {
		        delwin(win1);
			endwin();
			system("clear");
			break;
		}
	}
/**/
 /*
**_end_debug_and_failed_attempts_************************************************************/

//	delwin(win1);
//	endwin();
//	system("clear");
        return 0;
}



void waitms(int ms)
{
        struct timespec delay;
	if (ms>999){
	        delay.tv_sec = ms/1000;
	        delay.tv_nsec = (ms%1000)*1000000L;
	} else {
	        delay.tv_sec = 0;
	        delay.tv_nsec = (long)ms*1000000;
	}
        if (nanosleep(&delay,NULL)) {
                perror("nanosleep");
        }
}


void waitmics(int musec)
{
        struct timespec delay;
        delay.tv_sec = 0;
        delay.tv_nsec = musec*1000L;

        if (nanosleep(&delay,NULL)) {
                perror("nanosleep");
        }
}
