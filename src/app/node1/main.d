node1/main.d node1/main.o: node1/main.c node1/main.h ../library/eselib/include/global.h \
 ../library/eselib/include/node1.h
node1/main.o:
	avr-gcc -Wall -Wstrict-prototypes -Os -mmcu=atmega128 -ggdb -I../library/eselib/include -I../library/ttpa/include -I../library/scade/include -I./include -I./node1 -c -o node1/main.o node1/main.c
