all:mytbf

mytbf:tbfmain.o mytbf.o
	gcc $^ -o $@


clean:
	rm -rf *.o mytbf