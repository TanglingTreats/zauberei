build:
	@gcc -o sorcery main.c

run: build
	@./sorcery
