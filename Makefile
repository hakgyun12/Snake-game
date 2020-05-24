all:
	g++ -o snake src/fmain.cpp src/fsnakeGame.cpp -lncurses
	./snake