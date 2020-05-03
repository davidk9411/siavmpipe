compile:
	gcc -o siavm *.c

object:
	gcc -c *.c

debug:
	gcc -g -o siavm *.c

remove:
	rm -rf *.o siavm ./siavm.dSYM
