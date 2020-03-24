run: main.cpp game.h player.h utils.h menu.h
	g++ -o run main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
