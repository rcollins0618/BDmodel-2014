/*  Use Left, Right, and Up Arrow Keys! Now with more info!
Up for Debug / Values Check. 
Left for Mean line
Right for plot with incorrect variance values.    */


#include <ncurses.h>
#include <stdio.h>
#include <time.h>

void waitms(int);
void waitmics(int);

int main(void)
{
        WINDOW *win1;
        int x, y, i, j, k;	// j, k not used yet.
	int ridx;
	float ymufin=0.0, ftemp = 0.0, deltamu = 0.0, deltavar = 0.0;
	float ymu[3500] = {}, yvar[3500] = {};
	int xrnd[3500] = {}, yhts[100] = {};
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

	for (i=0; i<3500; i++) {	// xrnd[index]=rand(0:99)
		xrnd[i] = rand()%100;
	}				// end i.
	for (i=0; i<3500; i++) {				// i == xrnd[] index
		ridx = i-1;
		if (scnarr[0][xrnd[i]] == 1)
			break;
		for (j=34; j>=0; j--) {				// j == vertical scan index
			if (scnarr[j][xrnd[i]] != 1) {

/** */ 				for (k=0; k<=j; k++) {		// k == white drop space
					if (k>0)	mvwprintw(win1,k-1,xrnd[i]," ");
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
	}

	ymufin = ((float)ridx)/100;	// final y_mu

	for (i=0; i<=ridx; i++) {
		yhts[xrnd[i]]++;
		if (i < 1) {
			ymu[0] = 0.0;
			yvar[0] = 0.0;
		} else {
			ymu[i] = (float)(i)/100;
			yvar[i] = 0;
			for (j=0; j<100; j++) {
				ftemp = ((float)yhts[j] - ymu[i]);	//(xi-mu)
				ftemp = ftemp * ftemp;		//(xi-mu)^2
				yvar[i] =yvar[i]+ftemp;		//sum of squares
			}
			yvar[i] = yvar[i]/100;			//variance (indexed)
		//deltavar[i] = 
		}
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

//if KEY_LEFT, print generatied ballistic deposition map
		if (k == 68) {
			wclear(win1);
/* deposition	*/	for (i=0; i<100; i++) {
/* map:		*/		for (j=34; j>=0; j--) {
					if (scnarr[j][i] == 1)	mvwprintw(win1, j, i, "O");
				}
			}
/* average	*/	if (ymufin-((int)ymufin) < .5) {
/* indicator:	*/		for (i=0; i<100; i++) {
					if (scnarr[36-(int)ymufin][i]==0)
						mvwprintw(win1,36-ymufin,i,"-");
					if (scnarr[36-(int)ymufin][i]==1)
						mvwprintw(win1,36-ymufin,i,"X");
				}
				mvwprintw(win1,1,2,"y_mu: %f", ymufin);
				mvwprintw(win1,2,2,"y_mu_remainder < .5");
				mvwprintw(win1,35-(int)ymufin,0,"[%d]", (int)ymufin);
			} else {
				for (i=0; i<100; i++) {
					if (scnarr[35-(int)ymufin][i]==0)
						mvwprintw(win1,35-ymufin,i,"-");
					if (scnarr[35-(int)ymufin][i]==1)
						mvwprintw(win1,35-ymufin,i,"X");
				}
				mvwprintw(win1,1,2,"y_mu: %f", ymufin);
				mvwprintw(win1,2,2,"y_mu_remainder >= .5");
				mvwprintw(win1,34-(int)ymufin,0," [%d] ", (int)ymufin+1);
			}
			wrefresh(win1);

//if KEY_RIGHT, print statistics and graphs:
 		} else if (k == 67) {
			wclear(win1);
			mvwprintw(win1, 0, 0," N=%d \twidth=100\tmu_final=%.3f"
				"\t\tvar_final=%.3f",ridx,ymu[ridx],yvar[ridx]);
			mvwprintw(win1, 1, 0,"\t\t\t\tdelta_mu=%.5f\tdelta_var=%.5f"
				,(ridx,ymu[ridx]/ridx),(yvar[ridx]/ridx));
			//mvwprintw(win1, 1, 25, "delta_var[%d]=%.3f", ridx, 
			mvwprintw(win1, 3, 0," ymu[0:%d] (by 10%% bins):",ridx);
			mvwprintw(win1, 4, 0," yvar[0:%d] (by 10%% bins):",ridx);
			for (i=0; i<=10; i++) {
				ftemp = (float)ridx/10;
				mvwprintw(win1,34,i*9+3,"%02d%%",i*10);
				mvwprintw(win1,33-(int)ymu[(int)(i*ftemp)],i*9+2,"%.2f",
					ymu[(int)(i*ftemp)]);
				mvwprintw(win1,34-(int)ymu[(int)(i*ftemp)],i*9+2,"%.2f",
					yvar[(int)(i*ftemp)]);
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
