run: main.cpp game.h player.h utils.h menu.h map.h raygui.h config.cpp
	g++ -std=c++17 -o run main.cpp config.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
