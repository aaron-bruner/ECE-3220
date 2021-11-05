bin: tfs_1.c tfs_2.c tfs_driver1.c
	gcc -o tfs.out tfs_1.c tfs_2.c tfs_driver1.c

run:
	./a.out > out.txt

run2: tfs_1.c tfs_2.c tfs_driver2.c
	gcc tfs_1.c tfs_2.c tfs_driver2.c
	./a.out

debug: tfs_1.c tfs_2.c tfs_driver1.c
	gcc -g tfs_1.c tfs_2.c tfs_driver1.c
	gdb ./a.out

debug2: tfs_1.c tfs_2.c tfs_driver2.c
	gcc -g tfs_1.c tfs_2.c tfs_driver2.c
	gdb ./a.out

clean:
	rm *.out
