/*  This is a working version of project11.c  */
/*	This version gets KEY_RIGHT data for very large arrays!! 	*/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>	// "-lncurses" at compile time!
#include <time.h>
#include <math.h>	//sqrt! also "-lm" at compile time

#define W 100
#define H 1000
#define A 100000

void zeroit(int **array, int nrows, int ncols);
void waitms(int);
void waitmics(int);

int main(void)
{
        WINDOW *win1;
        int x, y, i, j, k, tmp; // j, k not used yet.
        int ridx;
        float ymufin=0.0, ftemp = 0.0, ftmp1 = 0.0, ftmp2 = 0.0, deltamu = 0.0, deltavar = 0.0;
        //float ymu[A], yvar[A];
        //int xrnd[A];
	int yhts[W];
        //int scnarr[H][A];
        int slowness = 0;

        initscr();
        cbreak();                               // Line buffering disabled
        keypad(stdscr, TRUE);                   // allow softkeys
        noecho();                               // don't display input when (w)getch();
        curs_set(0);                            // don't display the cursor.
        start_color();                          // enable color text
        init_pair(1,COLOR_RED,COLOR_BLACK);     // set color pair "1" as fg:RED, bg:BLK

        getmaxyx(stdscr,y,x);                   // find actual screen height and width
        win1 = newwin(y,x,0,0);         // use that for win1 (allow overflow without errors)

        srand((unsigned)(int)time(NULL));       // set random seed using current sec of day

	int **scnarr;				//initialize huge ass int array w/ malloc!
	scnarr = malloc(H * sizeof(int *));
	if (scnarr == NULL) {
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n");
		return 1;
	}
	for (i=0; i<H; i++){
		scnarr[i] = malloc(W * sizeof(int *));
		if (scnarr[i] == NULL){
			delwin(win1);
			endwin();
			system("clear");
			fprintf(stderr, "\nout of memory\n\n");
			return 1;
		}
	}

	float *ymu;
	ymu = malloc(A * sizeof(float *));
	if (ymu == NULL) {
		for (i=0; i<H; i++){ free(scnarr[i]); }
		free(scnarr);
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n\n");
	}

	float *yvar;
	yvar = malloc(A * sizeof(float *));
	if (yvar == NULL) {
		for (i=0; i<H; i++){ free(scnarr[i]); }
		free(scnarr);
		free(ymu);
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n\n");
	}

	int *xrnd;
	xrnd = malloc(A * sizeof(int *));
	if (ymu == NULL) {
		for (i=0; i<H; i++){ free(scnarr[i]); }
		free(scnarr);
		free(ymu);
		free(yvar);
		delwin(win1);
		endwin();
		system("clear");
		fprintf(stderr, "out of memory\n\n");
	}
	zeroit(scnarr,H,W);			// zero array. "cuz why not?"

	for(i=0; i<A; i++) {			// prepare list of random ints
		xrnd[i] = rand()%W;
	}

	for (i=0; i<A; i++) {			// i == xrnd[] index within loop
		ridx = i-1;			// ridx = xrnd[] index to use outside of loop
		tmp = xrnd[i];			// why? because "tmp" is shorter to spell
		if (scnarr[0][xrnd[i]])	// Tetris Mode: End drops when top is broken through
			break;		//  (This is also why ridx = i-1)
		for (j=0; j<H; j++) {
/* mode 1:	*/	if (j == H-1) {                 // If empty column (end of array),
/* empty column */		scnarr[j][tmp] = 1;     //  place block.

				yhts[xrnd[i]] = 1;	// inc. array keeping col. heights
				yvar[i] = 0;		// reset variance value for this loc
				for (k=0; k<W; k++) {	// calc xbar ("mu") and var
					ymu[i] = ymu[i]+(float)yhts[k]/W;
					ftemp = ((float)yhts[k] - ymu[i]);
					ftemp = ftemp * ftemp;
					yvar[i] = yvar[i]+ftemp;
				}
				ymufin = ymu[i];	// keep record of final xbar ("mu")
/* mode 2:		*/	} else if (
/* columns not empty:	*/		((tmp>0)&&(scnarr[j+1][tmp-1]==1)) ||
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
					break;
				}
			}
		}
	// } ? da fuq?

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
				ftmp2 = y*(int)sqrt( yvar[(int)(i*ridx/10)] )/sqrt(yvar[ridx]);

				if (i==0){
    /*  graph ymu[t]	*/	  mvwprintw(win1,y-3,2,"0.0");
				  wattron(win1,A_STANDOUT);
    /*  graph yvar[y]	*/	  mvwprintw(win1,y-2,2,"0.0");
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

//if ENTER, end:
		} else if (k == 10) {
			for (i=0; i<H; i++){ free(scnarr[i]); }
			free(scnarr);
			delwin(win1);
			endwin();
			system("clear");
			break;
		}
	}
	return 0;
}



void zeroit(int **scnarr, int nrows, int ncols)
{
	int i, j;
	for (i=0; i<H; i++) {
		for (j=0; j<W; j++) {
			scnarr[i][j]=0;
		}
	}
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
