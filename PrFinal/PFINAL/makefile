#DANIEL SEGURADO CLIMENT 48791680D
OPTS = -Wall -O3 -std=c++0x
mcp: mcp_bb.cc
	g++ ${OPTS} -o mcp_bb mcp_bb.cc

tar: mcp_bb.cc makefile
	tar cvfz 48791680D.tar.gz mcp_bb.cc makefile

clean:
	-rm mcp_bb
