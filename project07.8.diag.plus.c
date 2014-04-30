/* This is the diagonal "plus-symbol" version of project07.8.c
Please keep this window size to at least 100w x 35h as well. */

#include <ncurses.h>
#include <stdio.h>
#include <time.h>

void waitms(int);
void waitmics(int);

int main(void)
{
        WINDOW *win1;
        int x, y, i, j, k, tmp;	// j, k not used yet.
	int ridx;
	float ymufin=0.0, ftemp = 0.0, ftmp2 = 0.0, deltamu = 0.0, deltavar = 0.0;
	float ymu[3500] = {}, yvar[3500] = {};
	int xrnd[3500] = {}, yhts[100] = {};
	int scnarr[35][100] = {};
	int slowness = 0;

        initscr();
        cbreak();               // Line buffering disabled
        keypad(stdscr, TRUE);   // allow softkeys
        noecho();               // don't display input when (w)getch();
	curs_set(0);		// don't display the cursor.
        getmaxyx(stdscr,y,x);

	srand((unsigned)(int)time(NULL));

	win1 = newwin(y,x,0,0);

	for (i=0; i<3500; i++) {				// xrnd[index]=rand(0:99)
		xrnd[i] = rand()%100;
	}
	for (i=0; i<3500; i++) {				// i == xrnd[] index
		ridx = i-1;
		tmp = xrnd[i];
		if (scnarr[0][xrnd[i]])
			break;
		for (j=0; j<35; j++) {
			if (j == 34) {
				scnarr[j][tmp] = 1;

				yhts[xrnd[i]] = 1;
				yvar[i] = 0;
				for (k-0; k<100; k++) {
					ymu[i] = ymu[i]+(float)yhts[k]/100;
					ftemp = ((float)yhts[k] - ymu[i]);
					ftemp = ftemp * ftemp;
					yvar[i] = yvar[i]+ftemp;
				}
				ymufin = ymu[i];
				yvar[i] = yvar[i]+ftemp;

 				for (k=0; k<35; k++) {
					if (k>0)	mvwprintw(win1,k-1,tmp," ");
					mvwaddch(win1,k,tmp,ACS_PLUS);
					wrefresh(win1);
  // Enable This Line when running from localhost (not over ssh):
  // 					waitmics(slowness);
				}
			}else if (
					((tmp>0)&&(scnarr[j+1][tmp-1]==1)) ||
					(scnarr[j+1][tmp]==1) ||
					((tmp<99)&&(scnarr[j+1][tmp+1]==1))
				) {
				scnarr[j][tmp] = 1;

				yhts[tmp] = 35-j;
				yvar[i] = 0;
 				for (k=0; k<100; k++) {
					ymu[i] = ymu[i] + (float)yhts[k]/100;

					ftemp = ((float)yhts[k] - ymu[i]);
					ftemp = ftemp*ftemp;
					yvar[i] = yvar[i] + ftemp;
				}
				ymufin = ymu[i];
				yvar[i] = yvar[i]/100;

				for (k=0; k<=j; k++) {
					if (k>0)	mvwprintw(win1,k-1,tmp," ");
					mvwaddch(win1,k,tmp,ACS_PLUS);
					wrefresh(win1);
  // Enable This Line when running from localhost (not over ssh):
  //					waitmics(slowness);
				}
				break;
			}
		}
	}
	wrefresh(win1);

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

//if KEY_LEFT, print generatied ballistic deposition map
		if (k == 68) {
			wclear(win1);
/* deposition	*/	for (i=0; i<100; i++) {
/* map:		*/		for (j=34; j>=0; j--) {
					if (scnarr[j][i] == 1)	mvwaddch(win1,j,i,ACS_PLUS);

				}
			}
/* top value    */      for (i=0; i<100; i++) {
                                mvwaddch(win1,35-yhts[i],i,ACS_HLINE);
			}
/* average	*/	if (ymufin-((int)ymufin) < .5) {
/* indicator:	*/		for (i=0; i<100; i++) {
					if (scnarr[35-(int)ymufin][i]==0)
						mvwaddch(win1,36-ymufin,i,ACS_HLINE);
					if (scnarr[35-(int)ymufin][i]==1)
						mvwaddch(win1,36-ymufin,i,ACS_PLUS);
				}
				mvwprintw(win1,1,2,"y_mu: %.2f", ymufin);
				mvwprintw(win1,2,2,"y_mu_remainder < .5");
				mvwprintw(win1,35-(int)ymufin,0,"[%d]", (int)ymufin);
			} else {
				for (i=0; i<100; i++) {
					if (scnarr[34-(int)ymufin][i]==0)
						mvwaddch(win1,35-ymufin,i,ACS_HLINE);
					if (scnarr[34-(int)ymufin][i]==1)
						mvwaddch(win1,35-ymufin,i,ACS_PLUS);
				}
				mvwprintw(win1,1,2,"y_mu: %f", ymufin);
				mvwprintw(win1,2,2,"y_mu_remainder >= .5");
				mvwprintw(win1,34-(int)ymufin,0,"[%d]", (int)ymufin+1);
			}
			wrefresh(win1);

//if KEY_RIGHT, print statistics and graphs:
 		} else if (k == 67) {
			wclear(win1);
    /*	headers	*/	mvwprintw(win1, 0, 0," N=%d \twidth=100\tmu_final=%.3f"
				"\t\tvar_final=%.3f",ridx,ymu[ridx],yvar[ridx]);
			mvwprintw(win1, 1, 0,"\t\t\tdelta_mu=%.5f\tdelta_var=%.5f"
				,(ridx,ymu[ridx]/ridx),(yvar[ridx]/ridx));
			mvwprintw(win1, 3, 0," ymu[0:%d] (by 10%% bins):",ridx);
			mvwprintw(win1, 4, 0," yvar[0:%d] (by 10%% bins):",ridx);

			for (i=0; i<=10; i++) {
				ftemp = (float)ridx/10;
				ftemp = 35*(int)ymu[(int)(i*ridx/10)]/ymu[ridx];
				ftmp2 = 35*(int)yvar[(int)(i*ridx/10)]/yvar[ridx];
    /* 	graph ymu[t]	*/	mvwprintw(win1,33-ftemp,i*9+2,"%.2f",ymu[(int)(i*ridx/10)]);
				wattron(win1,A_STANDOUT);
    /*  graph yvar[y]   */	mvwprintw(win1,34-ftmp2,i*9+2,"%.2f",yvar[(int)(i*ridx/10)]);
				wattroff(win1,A_STANDOUT);
    /*  plot x axis(%)  */ 	mvwprintw(win1,34,i*9+3,"%02d%%",i*10);
			}
			wrefresh(win1);

//if KEY_UP, debug:
		} else if (k==65) {
			wclear(win1);
			mvwprintw(win1,0,0,"i:");
			mvwprintw(win1,7,0,"yhts[i]:\t(final)");
			mvwprintw(win1,14,0,"ymu[1:%d]:\t(ymu, %%-wise)",ridx);
			mvwprintw(win1,21,0,"yvar[0:%d]:\t(yvar, %%-wise)",ridx);
			for (i=0; i<100; i++){
				mvwprintw(win1,1+i/20,(i%20)*5," %2d",i);
				mvwprintw(win1,8+i/20,(i%20)*5," %2d",yhts[i]);
				mvwprintw(win1,15+i/20,(i%20)*5,"%4.1f",
					ymu[((i+1)*ridx)/100]);
				mvwprintw(win1,22+i/20,(i%20)*5,"%4.1f",
					yvar[((i+1)*ridx)/100]);
			}
			wrefresh(win1);


//if ENTER, end:
		} else if (k == 10) {
		        delwin(win1);
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
