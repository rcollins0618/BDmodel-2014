BalllisticDepositionModel-2014
==============================

Class Project for Applied Probability


PURPOSE:
* This code is a simple Ballistic Deposition simulation. It is meant to show how the mean and the variance (or roughness) of the surface of a randomly deposited material changes over time. It assumes each deposited block is equally cohesive (to itself) and adhesive to the flat surface it falls upon. It assumes: rand() is truly random (or close enough), particles fall orthagonal to the floor (no angular attack or variation from the Normal line), and no physics is taken into account (acceleration, mass, elasticity, etc.).

* The final purpose is to show that at a certain critical height given a system width, the "surface roughness" or "interface roughness" (Standard Deviation or Variance of the heights of individual columns of blocks) stops growing at either an exponential or linear rate, and finally levels off to a nearly stable value (given its own random walk). The only problem with this is that by the time I had to present my project, I didn't get the results I wanted. In fact, even with a large system size (see project13.c), the surface roughness seems to be widely different between runs.  Later, I found that this should be resolved if I had run an ensemble (many parallel runs) whose own value of necessary parallel runs may be determined by the initial system size. I did not get that far. But if you compile and run project13.c a thousand times and take the mean of all output values, that should be fair enough to give you want one would expect!




IMPORTANT NOTES:

* This code was first and foremost made and commented with myself as the only intended audience. The entire project is not one complete whole thing of any kind, but some instances of code may have features that I developed but didn't finally implement. Thus, it should have all the ingredients for a final polished example, but you won't find one. In fact, project13.c is not a final example, but an extended example that allows you to use bigger arrays, but at the cost of not being able to show the BD Model generated.
  (NOTE TO SELF: do this over the summer so you don't forget everything before you want to show someone!)

* project13.c and project0708.horiz.blk.c are the two safe / recommended versions to try if you want output that makes sense, but please feel free to modify and explore the code at will. These are the two that I used for my demonstration (with much explaining of the project first and foremost).

* It is suggested that you edit the code in a screen that is at least 120 characters wide, and run the code in a terminal that is at least 100 characters wide by 35 characters in height.  Some versions adapt the output to the screen size, but 100w x 35h is the assumed minimum "resolution".

* The value for Variance in most of the examples (except for project13.c and project0708.horiz.blk.c) is completely incorrect everywhere you will see it. All you have to do to get the right variance is compute the mean before computing the variance. This requires copying the for() loop used for the mean and variance, and using one for each. It takes longer to explain than to fix, so I may do that (don't bet on it though).

* It also may not be clear, but in project 13.c, what is shown alongside the mean values (after it is ran and you press the right arrow to get to the correct screen) is not the variance, but the standard deviation (sqrt(variance)) that is highlighted on the screen.

* Each version of code in this repository assumes a linux (or BSD) environment because that's what it has been compiled and tested on previously. If I wanted to use Windows, I wouldn't have used ncurses.




HOW TO COMPILE:

  gcc project[*].c -lm -lncurses
  
(This assumes gcc and the ncurses library is installed. For ubuntu, a simple "sudo apt-get install gcc libncurses5" should do.)




HOW TO USE:

* Compile the source code you wish to run.
* Resize the terminal screen to at least 100 char's wide by 35 char's high.
* Run the file you compiled (eg. a.out).
* Press the arrow keys to see if any different output is generated.
* Read through the code to get a better understanding of what it does.
* Some versions may require different screen sizes. Some may take up to a minute or so to generate the array before any output is shown on the screen. These same ones (eg. project13.c) also may require you to press the right arrow key before any output is shown to the screen.


PROTIPS:

* The source code should be written in a way that makes sense if you understand how to get the average (mean) of a set of integers and if you know how to get the variance of the same set of integers. The wikipedia entries make this cake.
* You may contact me about this code with any questions at collinrp_at_sunyit_dot_edu.
