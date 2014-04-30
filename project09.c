/*	This is the resizable version that still displays to screen	*/
/*	Just change the #define's below to customize!	*/
/* !!ALSO!! THIS IS THE COLORED BAND version. Beware the slowness!  */


#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#define W 200		// Screen Width
#define H 68		// Screen Height
#define A 13600		// Screen Area

void waitms(int);
void waitmics(int);

int main(void)
{
        WINDOW *win1;
        int x, y, i, j, k, tmp;	// j, k not used yet.
	int ridx;
	float ymufin=0.0, ftemp = 0.0, ftmp2 = 0.0, deltamu = 0.0, deltavar = 0.0;
	float ymu[A], yvar[A];
	int xrnd[A], yhts[W];
	int scnarr[H][A];
	int slowness = 0;

        initscr();
        cbreak();               // Line buffering disabled
        keypad(stdscr, TRUE);   // allow softkeys
        noecho();               // don't display input when (w)getch();
	curs_set(0);		// don't display the cursor.
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
        getmaxyx(stdscr,y,x);

	srand((unsigned)(int)time(NULL));

	win1 = newwin(y,x,0,0);

	for (i=0; i<A; i++) {				// xrnd[index]=rand(0:99)
		xrnd[i] = rand()%W;
	}
	for (i=0; i<A; i++) {				// i == xrnd[] index
		ridx = i-1;
		tmp = xrnd[i];
		if (scnarr[0][xrnd[i]])
			break;
		for (j=0; j<H; j++) {
			if (j == H-1) {
				scnarr[j][tmp] = 1;

				yhts[xrnd[i]] = 1;
				yvar[i] = 0;
				for (k-0; k<W; k++) {
					ymu[i] = ymu[i]+(float)yhts[k]/W;
					ftemp = ((float)yhts[k] - ymu[i]);
					ftemp = ftemp * ftemp;
					yvar[i] = yvar[i]+ftemp;
				}
				ymufin = ymu[i];
				yvar[i] = yvar[i]+ftemp;

 				for (k=0; k<H; k++) {
					if (k>0)	mvwprintw(win1,k-1,tmp," ");
					mvwaddch(win1,k,tmp,ACS_CKBOARD);
					wrefresh(win1);
  // Enable This Line when running from localhost (not over ssh):
  // 					waitmics(slowness);
				}
			}else if (
					((tmp>0)&&(scnarr[j+1][tmp-1]==1)) ||
					(scnarr[j+1][tmp]==1) ||
					((tmp<W-1)&&(scnarr[j+1][tmp+1]==1))
				) {
				scnarr[j][tmp] = 1;

				yhts[tmp] = H-j;
				yvar[i] = 0;
 				for (k=0; k<W; k++) {
					ymu[i] = ymu[i] + (float)yhts[k]/W;

					ftemp = ((float)yhts[k] - ymu[i]);
					ftemp = ftemp*ftemp;
					yvar[i] = yvar[i] + ftemp;
				}
				ymufin = ymu[i];
				yvar[i] = yvar[i]/W;

				for (k=0; k<=j; k++) {
					if ((i/500)%2 == 1){
						wattron(win1,COLOR_PAIR(1) | A_BOLD);
						if (k>0)	mvwprintw(win1,k-1,tmp," ");
						mvwaddch(win1,k,tmp,ACS_CKBOARD);
						wrefresh(win1);
						wattroff(win1,COLOR_PAIR(1) | A_BOLD);
					} else {
						if (k>0)	mvwprintw(win1,k-1,tmp," ");
						mvwaddch(win1,k,tmp,ACS_CKBOARD);
						wrefresh(win1);
					}
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
/* deposition	*/	for (i=0; i<W; i++) {
/* map:		*/		for (j=H-1; j>=0; j--) {
					if (scnarr[j][i] == 1)	mvwaddch(win1,j,i,ACS_CKBOARD);

				}
			}
/* top value    */      for (i=0; i<W; i++) {
                                mvwaddch(win1,H-yhts[i],i,ACS_HLINE);
			}
/* average	*/	if (ymufin-((int)ymufin) < .5) {
/* indicator:	*/		for (i=0; i<W; i++) {
					if (scnarr[H-(int)ymufin][i]==0)
						mvwaddch(win1,H+1-ymufin,i,ACS_HLINE);
					if (scnarr[H-(int)ymufin][i]==1)
						mvwaddch(win1,H+1-ymufin,i,ACS_CKBOARD);
				}
				mvwprintw(win1,1,2,"y_mu: %.2f", ymufin);
				mvwprintw(win1,2,2,"y_mu_remainder < .5");
				mvwprintw(win1,H-(int)ymufin,0,"[%d]", (int)ymufin);
			} else {
				for (i=0; i<W; i++) {
					if (scnarr[H-1-(int)ymufin][i]==0)
						mvwaddch(win1,H-ymufin,i,ACS_HLINE);
					if (scnarr[H-1-(int)ymufin][i]==1)
						mvwaddch(win1,H-ymufin,i,ACS_CKBOARD);
				}
				mvwprintw(win1,1,2,"y_mu: %f", ymufin);
				mvwprintw(win1,2,2,"y_mu_remainder >= .5");
				mvwprintw(win1,H-1-(int)ymufin,0,"[%d]", (int)ymufin+1);
			}
			wrefresh(win1);

//if KEY_RIGHT, print statistics and graphs:
 		} else if (k == 67) {
			wclear(win1);
    /*	headers	*/	mvwprintw(win1, 0, 0," N=%d \twidth=%d\tmu_final=%.3f"
				"\t\tvar_final=%.3f",ridx,W,ymu[ridx],yvar[ridx]);
			mvwprintw(win1, 1, 0,"\t\t\tdelta_mu=%.5f\tdelta_var=%.5f"
				,(ridx,ymu[ridx]/ridx),(yvar[ridx]/ridx));
			mvwprintw(win1, 3, 0," ymu[0:%d] (by 10%% bins):",ridx);
			mvwprintw(win1, 4, 0," yvar[0:%d] (by 10%% bins):",ridx);

			for (i=0; i<=10; i++) {
				ftemp = (float)ridx/10;
				ftemp = H*(int)ymu[(int)(i*ridx/10)]/ymu[ridx];
				ftmp2 = H*(int)yvar[(int)(i*ridx/10)]/yvar[ridx];
    /* 	graph ymu[t]	*/	mvwprintw(win1,H-2-ftemp,i*9+2,"%.2f",ymu[(int)(i*ridx/10)]);
				wattron(win1,A_STANDOUT);
    /*  graph yvar[y]   */	mvwprintw(win1,H-1-ftmp2,i*9+2,"%.2f",yvar[(int)(i*ridx/10)]);
				wattroff(win1,A_STANDOUT);
    /*  plot x axis(%)  */ 	mvwprintw(win1,H-1,i*9+3,"%02d%%",i*10);
			}
			wrefresh(win1);

//if KEY_UP, debug:
		} else if (k==65) {
			wclear(win1);
			mvwprintw(win1,0,0,"i:");
			mvwprintw(win1,7,0,"yhts[i]:\t(final)");
			mvwprintw(win1,14,0,"ymu[1:%d]:\t(ymu, %%-wise)",ridx);
			mvwprintw(win1,21,0,"yvar[0:%d]:\t(yvar, %%-wise)",ridx);
			for (i=0; i<W; i++){
				mvwprintw(win1,1+i/20,(i%20)*5," %2d",i);
				mvwprintw(win1,8+i/20,(i%20)*5," %2d",yhts[i]);
				mvwprintw(win1,15+i/20,(i%20)*5,"%4.1f",
					ymu[((i+1)*ridx)/W]);
				mvwprintw(win1,22+i/20,(i%20)*5,"%4.1f",
					yvar[((i+1)*ridx)/W]);
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
