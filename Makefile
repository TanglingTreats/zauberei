CC = gcc
CFLAGS = -Wall -Werror -I. -std=c99
RM= rm -f

all: zauberei

clean:
	$(RM) *.o

%: %.o
	$(CC) -o $@ $<

zauberei.o: zauberei.c
	$(CC) -c zauberei.c

run: all
	@./zauberei
