# The makefile for asg1.
# Type:
#   make         -- to build program asg1
#   make clean   -- to delete object files and executable

asg1 : asg1.o child_function.o
	gcc -Wall -g asg1.o -o asg1 -pthread
	gcc -Wall -g child_function.o -o child_function

clean :
	rm -f *.o a.out
