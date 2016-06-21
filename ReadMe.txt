========================================================================
PopStarSolver Project Overview
========================================================================

	Pop Star is game where you have (generally speaking) a 10x10 grid of 4 different 
colors. If two or more adjacent blocks match color you can "pop" (remove from board)
those blocks. The board then collapses (top to bottom and right to left). The more 
blocks that are connected, the higher the points you get. There is also a bonus depending 
the number of remaining blocks when you're out of "pops"

The really interesting thing about the game is that, due to the scoring mechanism. what
seems like a really poor set of pops can end up being better in the long run (e.g. doing a 
series of small 1x2 pops to connect together a large chunk for a high score.

The scoring mechansim (for at least the version I'm using) works like this:

1: cannot pop
2: 20 points 
at this point with start an an add of 25 which increasess by 10 every iteration
3: 45 points (+25)
4: 80 points (+35)
5- 120 points (+45)
...
100: 50,000

There is also a bonus for clearing the board. It starts at 2 thousand and removes
20 + 40 per iteration amount from the previous value:
0 remaining: 2000 points
1 remaining: 1980 points (-20 from previous value)
2 remaining: 1920 points (-60 from previous value)
3 remaining: 1820 points (-100 from previous value)
...
9 remaining: 380 points
10+: none

As you can see this makes for some interesting solutions. Are you better off trying to create
a large chunk to pop without regard to clearing the board or are you better off trying to clear 
the board. Both would be ideal but not always possible.

I'm going to start by modeling the board which I'll start of with a 2d array. Mostly
for future purposes.  It could just as easily be a single array. 

The PopStarSolver project is just a basic windows console app I'm using for testing. The Solver project does the work.

Right now, using the multi-threaded version of the solver it generates a little over 200k solutions a second. After a 
few seconds you'll have a pretty good solution. However, I've never been able to complete an entire board to find the absolute *best*
solution since there literallly billions (if not more depending on the board). I've come up with some ideas to make it faster but that
comes at the cost of memory and I really don't have enough to support those solutions.
