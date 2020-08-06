snake: clean
	cc snake.c -o snake -lncurses
clean:
	rm -f snake
