CC = g++

all:
	$(CC) 'game.cpp' -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system -g -o a

clean:
	rm -f a
