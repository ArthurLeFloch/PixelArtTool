CFLAGS = -Wall -Wextra -std=c99 -g3
OTHERS = palette.o utils.o

vpath % src

all: project

%.o: %.c
	cc -c $(CFLAGS) $< -I src/

project: $(OTHERS)
	gcc -c src/main.c $(sdl2-config --cflags --libs)
	gcc -o project $(OTHERS) main.o -lSDL2 -lm

clean:
	rm -f *.o
	rm -f *.ppm
