all: clean
	gcc -c hash_table.c
	gcc -c prime.c
	gcc -o hashtable hash_table.o prime.o -lm
	./hashtable

clean:
	rm -f hashtable.exe
