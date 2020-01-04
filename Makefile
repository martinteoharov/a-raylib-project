run: main.cpp game.h
	g++ -o run main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
