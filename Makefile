CC = g++

all:
	$(CC) 'game.cpp' -lsfml-graphics -lsfml-window -lsfml-system -g -o a

clean:
	rm -f a
