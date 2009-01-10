LIBTCOD = -ltcod
LIBLUA = -llua5.1
LIBINIPARSER = -liniparser

CC = gcc
CFLAGS = -Wall -pedantic -ansi
LIBS = $(LIBTCOD) $(LIBLUA) $(LIBINIPARSER)
OBJ = externs.o main.o game.o map.o script.o misc.o item.o config.o api.o player.o
OUT = psionrl

$(OUT): $(OBJ) psionrl.h
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	
main.o: main.c
	$(CC) -c $<
	
externs.o: externs.c
	$(CC) -c $<
	
config.o: config.c
	$(CC) -c $<
	
game.o: game.c
	$(CC) -c $<
	
player.o: player.c
	$(CC) -c $<
	
map.o: map.c
	$(CC) -c $<
	
script.o: script.c
	$(CC) -c $<
	
api.o: api.c
	$(CC) $(CFLAGS) -c $<
	
misc.o: misc.c
	$(CC) -c $<
	
item.o: item.c
	$(CC) -c $<
	
.PHONY: clean
clean:
	rm -rf *.o

.PHONY: build
build:
	rm -rf ../build
	mkdir ../build
	cp $(OUT) ../build
	cp *.dll ../build
	cp -r res ../build
	cp -r lua ../build
