CC = gcc
LIBS = -lncurses

main: clean snake.o food.o
	$(CC) main.c -o snake $(LIBS) snake.o food.o

snake.o: snake.c node.h
	$(CC) -c snake.c

food.o: food.c node.h
	$(CC) -c food.c

clean:
	rm -f snake *.o
