CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g
TARGET  = dotsboxes
OBJS    = main.o game.o board.o display.o
PREFIX  = /usr/local

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h board.h display.h
	$(CC) $(CFLAGS) -c game.c

board.o: board.c board.h
	$(CC) $(CFLAGS) -c board.c

display.o: display.c display.h board.h
	$(CC) $(CFLAGS) -c display.c

install: $(TARGET)
	install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all install valgrind clean
