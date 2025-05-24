#DANIEL SEGURADO CLIMENT 48791680D
OPTS = -Wall -O3 -std=c++0x
maze_bb: maze_bb.cc
	g++ ${OPTS} -o maze_bb maze_bb.cc

tar: maze_bb.cc makefile
	tar cvfz 48791680D.tar.gz maze_bb.cc makefile memoria.pdf

clean:
	-rm maze_bb