all:
	g++ -o snake src/main.cpp src/snakeGame.cpp -lncurses
	./snake