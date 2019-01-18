
all: frogger

frogger: main.c frogger.c threadwrappers.c log.c player.c
	clang -Wall -o frogger main.c threadwrappers.c log.c frogger.c console.c player.c -lcurses -pthread 

clean:
	rm -rf frogger


