OPTS = -Wall -Wextra -O3 -std=c++0x

all: quickSort quickSort.CPUtimes graphs

graphs: quickSort.CPUtimes quickSort.gpi
	gnuplot quickSort.gpi

quickSort: quickSort.cc
	g++ ${OPTS} -o quickSort quickSort.cc	 

quickSort.CPUtimes: quickSort
	./quickSort > quickSort.CPUtimes

tar: quickSort.cc quickSort.gpi makefile
	tar cvfz 48791680D.tar.gz quickSort.cc quickSort.gpi makefile

clean: 
	rm -f quickSort quickSort.CPUtimes fit.log quickSort*.png *~
 
