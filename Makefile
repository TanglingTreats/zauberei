CC = gcc
CFLAGS = -Wall -I. -std=c99
RM= rm -f
DEPS= lexer.h compiler.h
OBJ = zauberei.o lexer.o compiler.o

all: zauberei

clean:
	$(RM) *.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

zauberei: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: all
	@./zauberei
