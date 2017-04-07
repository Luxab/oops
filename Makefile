CC = g++

all:
	$(CC) 'game.cpp' -lsfml-graphics -lsfml-window -lsfml-system -g -o runme

clean:
	rm -f runme
