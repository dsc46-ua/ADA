#DANIEL SEGURADO CLIMENT 48791680D
OPTS = -Wall -O3 -std=c++0x
mcp: mcp_bt.cc
	g++ ${OPTS} -o mcp_bt mcp_bt.cc

tar: mcp_bt.cc makefile
	tar cvfz 48791680D.tar.gz mcp_bt.cc makefile

clean:
	-rm mcp_bt
