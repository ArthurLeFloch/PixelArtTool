WIDTH ?= 1400
HEIGHT ?= 900
OPT = -D WIDTH=$(WIDTH) -D HEIGHT=$(HEIGHT)
CFLAGS = -Wall -Wextra -std=c99 -g3 $(OPT)
OTHERS = colors.o utils.o render.o pixel_art.o

vpath % src

all: project

%.o: %.c
	gcc -c $(CFLAGS) $< -I src/

project: $(OTHERS)
	gcc $(OPT) -c src/main.c $(sdl2-config --cflags --libs)
	gcc $(CFLAGS) -o project $(OTHERS) main.o -lSDL2 -lSDL2_ttf -lm

clean:
	rm -f project
	rm -f *.o
