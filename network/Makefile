C_SOURCES = $(wildcard matrix/*.c neural/*.c util/*.c *.c)
HEADERS = $(wildcard matrix/*.h neural/*.h util/*.h *.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -O3 -lSDL2_image -lSDL2 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

MAIN = main
CC = gcc
LINKER = /usr/bin/ld


main: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ -lm

# Generic rules
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@ -lm

clean:
	rm matrix/*.o *.o neural/*.o util/*.o ${MAIN}
