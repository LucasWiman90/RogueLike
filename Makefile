CFLAGS = -Wall
NCURSES_FLAGS = -lncurses

all: main

main: main.c
	echo "Compiling the main file"
	gcc $(CFLAGS) main.c -o main $(NCURSES_FLAGS)

clean:
	echo "Removing everything but the source files"
	rm main