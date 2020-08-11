snake: clean
	cc main.c -o snake -lncurses
clean:
	rm -f snake
