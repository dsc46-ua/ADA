#DANIEL SEGURADO CLIMENT 48791680D
OPTS = -Wall -O3 -std=c++0x
maze: maze_bt.cc
	g++ ${OPTS} -o maze_bt maze_bt.cc

tar: maze_bt.cc makefile
	tar cvfz 48791680D.tar.gz maze_bt.cc makefile

clean:
	-rm maze_bt
