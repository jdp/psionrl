TCOD = -ltcod-mingw
LUA = -llua

CC = gcc
CFLAGS = -Wall -pedantic
LIBS = $(TCOD) $(LUA)
OBJ = main.o externs.o game.o map.o script.o misc.o item.o config.o api.o
OUT = psionrl.exe

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	
main.o: main.c common.h
	$(CC) -c $<
	
externs.o: externs.c externs.h
	$(CC) -c $<
	
config.o: config.c config.h
	$(CC) -c $<
	
game.o: game.c game.h
	$(CC) -std=c99 -c $<
	
map.o: map.c map.h
	$(CC) -c $<
	
script.o: script.c script.h
	$(CC) -c $<
	
api.o: api.c api.h
	$(CC) -c $<
	
misc.o: misc.c misc.h
	$(CC) -c $<
	
item.o: item.c item.h
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