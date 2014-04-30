/* The finished working large-array version!!!  */

/*______________________________________________________;
								
	Edit this file in a screen at least:		; 	
							;	
		   120 chars wide:			;	
							;	
_____STRETCH_UNTIL_THIS_LINE_DISPLAYS_AS_ONE_LINE____________________________________________________________________;
								
________________________________________________________;	
								
	standard compile:				;	
							;	
		 gcc project13.c -lm -lncurses -o13.c	;	
							;	
		      -lm == 	link: math		;	
		-lncurses == 	link: ncurses		;	
________________________________________________________;	
								
								

										
_______!!!_NExT_!!!_____________________________________________________;	
										
	When this is complete, and you're ready for project 14.c:	;	
									;	
	format for STUDENT_T distribution?     				;	
		* change "ymu" to "hbar"				;	
		* find ymu						;	
		* use that to change your output AND			;	
			find rate of error				;	
________________________________________________________________________;	
										

*/

#include <stdio.h>	// for outputting errors with printf(), stderr(), etc.
#include <stdlib.h>	// for struct, malloc(), system(), rand()
#include <time.h>	// for timespec()s in waitms and waitmics
#include <ncurses.h>	// for animations and greater control over formatted screen output
#include <math.h>	// for sqrt() function used for RMS or Std. Dev.

#define W 800		// EDIT HERE ONLY to change system width (nbins)
#define H 2000		// EDIT HERE ONLY to change system height break point (stop at this height!)
#define A W*H		// system area

void zeroit(int **array, int nrows, int ncols);
void waitms(int);
void waitmics(int);

int main(void)
{
	WINDOW *win1;
	int x, y, i, j, k, tmp=0, ridx=0;
	float ymufin=0.0, yvarfin=0.0, yvarmax=0.0, ftemp=0.0, ftmp1=0.0, ftmp2=0.0;
	int yhts[W];

// initialize ncurses variables:
	initscr();
	cbreak();		// Line buffering disabled
	keypad(stdscr, TRUE);	// allow keys like arrows and pgup pgdn
	noecho();		// don't display input when (w)getch();
	curs_set(0);		// don't display cursor
	start_color();		// enable color, and:
	init_pair(1, COLOR_RED, COLOR_BLACK);	// set color pair '1' as fg:RED, bg:BLK

	getmaxyx(stdscr,y,x);	// set y and x as current screen height and width!!!!
	win1 = newwin(y,x,0,0);	//use those values for custom window (other than stdscr)
	//keypad(win1, TRUE);	// attempt ability to get extra key values for 'win1'
// :end ncurses settings

	srand((unsigned int)time(NULL));	//set random seed using currnet second of day

// initialize potentially large arrays using malloc's with error checking and free()::

	int **bdarr;						// Ballistic Deposition Array:
	bdarr = malloc(H * sizeof(int *));			// int bdarr[H][W]
	if (bdarr == NULL) {
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of virtual memory\n");
		return 1;
	}
	for (i=0; i<H; i++){
		bdarr[i] = malloc(W * sizeof(int *));
		if (bdarr[i] == NULL){
			delwin(win1);
			endwin();
			system("clear");
			fprintf(stderr, "\nout of virtual memory\n\n");
			return 1;
		}
	}

	float *ymu;						// y heights xbar
	ymu = malloc(A * sizeof(float *));			// float ymu[A]
	if (ymu == NULL) {
		for (i=0; i<H; i++){ free(bdarr[i]); }
		free(bdarr);
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n\n");
	}

	float *yvar;						// y heights variance
	yvar = malloc(A * sizeof(float *));			// float yvar[A]
	if (yvar == NULL) {
		for (i=0; i<H; i++) { free(bdarr[i]); }
		free(bdarr);
		free(ymu);
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n\n");
	}

	float *intw;						// interface width (y ht.s std. dev.)
	intw = malloc(A * sizeof(float *));			// float intw[A]
	if (intw == NULL) {
		for (i=0; i<H; i++) { free(bdarr[i]); }
		free(bdarr);
		free(ymu);
		free(yvar);
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n\n");
	}

	int *xrnd;						// int xrnd[A]
	xrnd = malloc(A * sizeof(int *));
	if (ymu == NULL) {
		for (i=0; i<H; i++){ free(bdarr[i]); }
		free(bdarr);
		free(ymu);
		free(yvar);
		free(intw);
		delwin(win1);
		endwin();
		system("clear");
	}
	zeroit(bdarr,H,W);		// is this required or just superstition?
// ::end initializing large arrays!!

// initialize xrnd[A] to be used to hold all possible random values used! ::
// 	also zeroit( ymu[A], yvar[A], intw[A] ) ::
	for (i=0; i<A; i++) {
		xrnd[i] = rand()%W;
		 ymu[i] = 0;
		yvar[i] = 0;
		intw[i] = 0;
	}
	for (i=0; i<W; i++) {
		yhts[i] = 0;
	}
// :: end init xrnd, and zeroit (ymu, yvar, intw)

// generate bdarr[H][W], yhts[W], ymu[0:ridx], yvar[0:ridx], intw[0:ridx] ::
	for (i=0; i<A; i++) {
		ridx = i-1;		// save number of used xrnd[]s
		if (bdarr[0][xrnd[i]])	{
			 break;		// "Tetris Mode" End when top of array is breached
		}
/* case:    */	for (j=0; j<H; j++) {	// scan from top down
/* a: (bottom)	*/	if (j == H-1) {	// if at bottom of array, and no block below exists..
/*	yhts[W]	*/		yhts[xrnd[i]] = 1;	// inc height of y at location of block
/* b: (else)	*/	} else if (
				  ( (xrnd[i]>0)&&(bdarr[j][xrnd[i]-1]==1) ) ||	// if block to left exists..
				  (bdarr[j+1][xrnd[i]]==1) ||			// if block below exists..
				  ( (xrnd[i]<W-1)&&(bdarr[j][xrnd[i]+1]==1) )	// if block to right exists..
			) {
/*	yhts[W]	*/		yhts[xrnd[i]] = H-j;			//inc yhts[W] @ location xrnd[i]
			}
/* (a OR b):	*/	if (
			   (j == H-1) ||		// if at bottom of array and no block below exists..
			   (xrnd[i]>0)&&(bdarr[j][xrnd[i]-1]==1) ||	// OR block to left exists..
			   (bdarr[j+1][xrnd[i]]==1) ||			// OR block below exists..
			   (xrnd[i]<W-1)&&(bdarr[j][xrnd[i]+1]==1)	// OR block to right exists..
			) {
/*  bdarr[H][W]	*/		bdarr[j][xrnd[i]] = 1;				//..place block

/*	ymu[A]	*/		for (k=0; k<W; k++) {
					ymu[i] = ymu[i] + (float)yhts[k]/W;	// scan yhts[W], get ymu[A] @ i
				}						// @i: ymu[A] = sum(yhts[W]) / W
/*	ymufin	*/		ymufin = ymu[i];				// final ymu (hbar)

/*	yvar[A]	*/		for (k=0; k<W; k++) {
					ftemp = ((float)yhts[k] - ymufin);	// k = 0:W-1, SUM(yhts(k)-hbar)
					ftemp = ftemp*ftemp;			// SShh(k) (SS of heights)
					yvar[i] = yvar[i] + ftemp;		// k = 0:W-1, SUM(SShh)
				}
				yvar[i] = yvar[i]/W;				// VAR[i](0:k)
				yvarfin = yvar[i];
				if (yvarmax < yvarfin){ yvarmax = yvarfin; }
/*	intw[A]	*/		intw[i] = sqrt(yvar[i]);			// std. dev. (or RMS)
				// note: is intw[i] centered around 0 or xbar?	== std. dev.? or RMS?
				//	 think about this later when you have time.
				break;
			}
		}
	}
// :: end generating values and BDarray





	while (1) {
		k = wgetch(win1);
		if (k == 27) {			//clear BS input, keep good stuff
			k = wgetch(win1);	//k == 91
			k = wgetch(win1);	//k==50s+getch: pgup/pgdn; k==60s up/dn/l/r
			if (k/10 == 5) wgetch(win1);	//ignore extra getch sent
		}

//if KEY_RIGHT, map data
		if (k == 67) {
			wclear(win1);
			delwin(win1);
        		getmaxyx(stdscr,y,x);		// find actual screen height and width
        		win1 = newwin(y,x,0,0);		// use that for win1 (allow overflow without errors)

    /*  headers */	mvwprintw(win1, 0, 0," N=%d \twidth=%d\tmu_final=%.3f"
				"\t\tvar_final=%.3f",ridx,W,ymu[ridx],yvar[ridx]);
			mvwprintw(win1, 1, 0,"\t\tdelta_mu=%.5f\tdelta_var=%.5f"
				,(ridx,ymu[ridx]/ridx),(yvar[ridx]/ridx));
			mvwprintw(win1, 3, 0," ymu[0:%d] (by 10%% bins):",ridx);
			mvwprintw(win1, 4, 0," sqrt(yvar[0:%d]) (by 10%% bins):",ridx);

			for ( i=0; i<=10; i++ ) {
				ftemp = (float)ridx/10;
				ftmp1 = y*(int)ymu[(int)(i*ridx/10)]/ymu[ridx];
				ftmp2 = y*(int)sqrt( yvar[(int)(i*ridx/10)] )/sqrt(yvarmax);

				if (i==0){
    /*  graph ymu[t]	*/	  mvwprintw(win1,y-3,2,"%.2f", ymu[0]);
				  wattron(win1,A_STANDOUT);
    /*  graph yvar[y]	*/	  mvwprintw(win1,y-2,2,"%.2f", yvar[0]);
				  wattroff(win1,A_STANDOUT);
    /*  plot x axis(%)	*/	  mvwprintw(win1,y-1,3,"0%%");
				} else if (i>0) {
    /*  graph ymu[t]	*/	  mvwprintw(win1,y-2-ftmp1,i*(x/11)+2,"%.2f",ymu[(int)(i*ridx/10)]);
				  wattron(win1,A_STANDOUT);
    /*  graph yvar[y]	*/	  mvwprintw(win1,y-1-ftmp2,i*(x/11)+2,"%.2f",sqrt(yvar[(int)(i*ridx/10)]));
				  wattroff(win1,A_STANDOUT);
    /*  plot x axis(%)	*/	  mvwprintw(win1,y-1,i*(x/11)+3,"%02d%%",i*10);
				}
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
                                mvwprintw(win1,15+i/20,(i%20)*5,"%3.0f",
                                        ymu[((i+1)*ridx)/100]);
                                mvwprintw(win1,22+i/20,(i%20)*5,"%3.0f",
                                        yvar[((i+1)*ridx)/100]);
                        }
                        wrefresh(win1);

//if ENTER, end:
		} else if (k == 10) {
			for (i=0; i<H; i++){ free(bdarr[i]); }
			free(bdarr);
			free(ymu);
			free(yvar);
			free(intw);
			delwin(win1);
			endwin();
			system("clear");
			break;
		}
	}
	return 0;



}



void zeroit(int **bdarr, int nrows, int ncols)
{
	int i, j;
	for (i=0; i<H; i++)
		for (j=0; j<W; j++)
			bdarr[i][j]=0;
}


void waitms(int ms)
{
	struct timespec delay;
	if (ms>999) {
		delay.tv_sec = ms/1000;
		delay.tv_nsec = (ms%1000)*1000000L;	//value must be less than 1Billion
	} else {
		delay.tv_sec = 0;
		delay.tv_nsec = (long)ms*1000000;	// also, see below for more info
	}
	if (nanosleep(&delay,NULL))
		perror("nanosleep"); 	// prints a descriptive error message to stderr
}


void waitmics(int musec)
{
	struct timespec delay;		// timespec defined by time.h
	delay.tv_sec = 0;		// platform-specific data type, but cast as you like
	delay.tv_nsec = musec*1000L;	// same as above, make sure at least "long"

	if (nanosleep(&delay,NULL))
		perror("nanosleep: ");	// prints a descriptive error message to stderr
}
