TCOD = -ltcod-mingw
LUA = -llua

CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LIBS = $(TCOD) $(LUA)
OBJ = main.o externs.o game.o map.o script.o misc.o item.o config.o api.o
OUT = psionrl.exe

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	
main.o: main.c psionrl.h
	$(CC) -c $<
	
externs.o: externs.c
	$(CC) -c $<
	
config.o: config.c
	$(CC) -c $<
	
game.o: game.c
	$(CC) -c $<
	
map.o: map.c
	$(CC) -c $<
	
script.o: script.c
	$(CC) -c $<
	
api.o: api.c
	$(CC) -c $<
	
misc.o: misc.c
	$(CC) -c $<
	
item.o: item.c
	$(CC) -c $<
	
.PHONY: clean
clean:
	rm -rf *.o
	
.PHONY: run
run:
	cd ../bin
	game

.PHONY: build
build:
	rm -rf ../build
	mkdir ../build
	cp $(OUT) ../build
	cp *.dll ../build
	cp -r res ../build
	cp -r lua ../build