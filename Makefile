# point this to wherever you installed libtcod
LIBTCOD = /home/justin/src/libtcod-1.4.1

CC = gcc
CFLAGS = -L$(LIBTCOD) -I$(LIBTCOD)/include
LIBS = -ltcod -llua5.1
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
	rm $(OUT)
	rm *.o

.PHONY: build
build:
	rm -rf ../build
	mkdir ../build
	cp $(OUT) ../build
	cp *.dll ../build
	cp -r res ../build
	cp -r lua ../build
