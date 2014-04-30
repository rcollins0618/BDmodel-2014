/*  Keypad decoding. (which is !different! for your own 
window than for stdscr. Also, another new screen!
Press left or right arrow keys.*/

#include <ncurses.h>
#include <stdio.h>
#include <time.h>

void waitms(int);
void waitmics(int);

int main(void)
{
        WINDOW *win1;
        int x, y, i, j, k;	// j, k not used yet.
	int xidx;
	float xmu[3500] = {}, xvar[3500] = {}, deltamu[3500] = {}, deltavar[3500] = {};
	float ymu[3500] = {}, yvar[3500] = {};
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
		if (scnarr[0][xrnd[i]] == 1)
			break;
		for (j=34; j>=0; j--) {				// j == vertical scan index
			if (scnarr[j][xrnd[i]] != 1) {

/** */ 				for (k=0; k<=j; k++) {		// k == white drop space
					if (k>0)
						mvwprintw(win1,k-1,xrnd[i]," ");
					mvwprintw(win1,k,xrnd[i],"O");
					wrefresh(win1);
  // Enable This Line when running from localhost (not over ssh):	
  //					waitmics(slowness);
				}
/*  **/
				scnarr[j][xrnd[i]] = 1;
		break;
			}
		}
		xidx = i;
		ymu[i] = (float)i/100;
	}
	wgetch(win1);
	wclear(win1);
	/*
***********************************************************************************************
	*/
	wrefresh(win1);
	while (1) {
		k = wgetch(win1);
		if (k == 27) {				//clear BS input, keep good stuff
			k = wgetch(win1); //k == 91
			k = wgetch(win1); //k==50s+getch: pgup/pgdn; k==60s up/dn/l/r
			if (k/10 == 5) wgetch(win1); //ignore extra getch sent
		}

		if (k == 68) {				//if KEY_LEFT:
			wclear(win1);			//  print generated ballistic dep.
			for (i=0; i<100; i++) {
				for (j=34; j>=0; j--) {
					if (scnarr[j][i] == 1)	mvwprintw(win1, j, i, "O");
				}
			}
			wrefresh(win1);
 		} else if (k == 67) {			//if KEY_RIGHT
			wclear(win1);			// print statistics and graphs
			mvwprintw(win1, 10, 10," xidx= %d", xidx);
			mvwprintw(win1, 11, 10," ymu = %f", ymu[xidx]);
			mvwprintw(win1, 13, 10," ymu[0:1500](by 100's): ");
			for (i=0; i<15; i++) {
				mvwprintw(win1, 14, i*6, "%.2f", ymu[i*100]);
			}
			wrefresh(win1);

		} else if (k == 10) {			//if ENTER
		        delwin(win1);			//  end
			endwin();
			system("clear");
			break;
		}
	}
	return k;
	/*
***********************************************************************************************
	*/
	delwin(win1);
	endwin();
	system("clear");
        return 0;
}

/*
***********************************************************************************************
*/


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
