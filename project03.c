#include <ncurses.h>
#include <stdio.h>
#include <time.h>

void waitms(int);
void waitmics(int);
int genarray(int []);

int main(void)
{
        WINDOW *win1;
        int y, x, i, j, k;	// j, k not used yet.
	int xrnd[3500] = {};
	int scnarr[35][100] = {};

        initscr();
        cbreak();               // Line buffering disabled
        keypad(stdscr, TRUE);   // allow softkeys
        noecho();               // don't display input when getch();
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
		for (j=34; j>=0; j--) {				// j == vertical scanner
			if (scnarr[j][xrnd[i]] != 1) {	
				scnarr[j][xrnd[i]] = 1;
				for (k=0; k<=j; k++) {		// k == white drop spaace
					if (k>0)
						mvwprintw(win1,k-1,xrnd[i]," ");
					mvwprintw(win1,k,xrnd[i],"O");
					wrefresh(win1);
					//waitmics(50);
				}
				break;
			}
		}
	}


/**_debug_and_test_code_**********************************************************************
 *	//for (j=0; j<35; j++) {			// j == vertical existence scanner.
 *		//for (i=0; i<100; i++) {		// i == row incrementer.
 *			//k = (i+j*100);		// k == xrnd[] incrementer.
 *			//mvwprintw(win1,33,10,"k = %d",k);
 *			//wrefresh(win1);
 *			//waitms(100);
 *			//if (scnarr[34-j][xrnd[k]] != 1) {
 *			//	mvwprintw(win1,j,xrnd[j][i],"X");
 *			//	wrefresh(win1);
 *			//	waitms(100);
 *			//}
 *		//}
 *	//}					// end i,j,k.
 *
**********************************************************************************************
 *
 *		//for (i=0; i<100; i++) {
 *			//if (!scnarr[j][xrnd[i]]) {
 *				//mvwprintw(win1,j,5,"%d\t%d\t%d",xrnd[i],j,i);
 * 				//waitms(1);
 *
 *				//wrefresh(win1);
 * 			//}
 * 		//}
 *
**********************************************************************************************
 *
 * 	mvwprintw(win1, y/2-2, 3, "Available Resolution: %d by %d chars", 
sizeof(xrnd)/4,sizeof(scnarr)/400);
 * 	mvwprintw(win1, y/2-2, 34, ", or %d \"pixels\"",sizeof(scnarr)/4);
 * 	mvwprintw(win1,y/2,x/2-2, "%d",xrnd[64]);
 * 	wrefresh(win1);
 *
**********************************************************************************************
 */
	waitms(1500);
	for (j=0; j<35; j++) {
		for (i=0; i<100; i++) {
			if (scnarr[j][i] == 1) {
				mvwprintw(win1,j,i,"X");
				wrefresh(win1);
			}
		}
	}
/*
**_end_debug_and_failed_attempts_************************************************************/

	waitms(1500);
        delwin(win1);
        endwin();
      //system("clear");
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


int genarray(int arr[])
{
	int out;
	out = sizeof(arr);
	return out;
}
