all: make

driver1: tfs_1.o tfs_2.o tfs.h tfs_driver1.o 
	gcc tfs_driver1.c tfs_1.c tfs_2.c -o driver1
	
driver2: tfs_1.o tfs_2.o tfs.h tfs_driver2.o 
	gcc tfs_driver2.c tfs_1.c tfs_2.c -o driver2
	
clean:
	rm driver1 driver2 *.o
	
tar:
	tar -czvf Homework3.tar.gz *
