CFLAGS = -Wall -lncurses

all: main.o

main.o: main.c
	echo "Compiling the main file"
	gcc $(CFLAGS) -c main.c

clean:
	echo "Removing everything but the source files"
	rm main.o