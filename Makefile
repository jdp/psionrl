LIBTCOD = -ltcod
LIBLUA = -llua5.1
LIBINIPARSER = -liniparser

CC = gcc
CFLAGS =
LIBS = $(LIBTCOD) $(LIBLUA) $(LIBINIPARSER)
SRC = externs.c main.c config.c api.c game.c map.c script.c misc.c item.c  \
      player.c psionics.c
OBJ = ${SRC:.c=.o}
OUT = psionrl

$(OUT): $(OBJ) psionrl.h
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
	
.c.o:
	$(CC) $(CFLAGS) -c $<
	
.PHONY: clean
clean:
	rm *.o

.PHONY: build
build:
	rm -rf ../build
	mkdir ../build
	cp $(OUT) ../build
	cp *.dll ../build
	cp -r res ../build
	cp -r lua ../build
