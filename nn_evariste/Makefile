C_SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -Wall -Wextra -O3 -fsanitize=address -g
LDLIBS = 

MAIN = main
CC = gcc
LINKER = /usr/bin/ld


main: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ -lm

# Generic rules
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@ -lm

clean:
	rm *.o ${MAIN}
