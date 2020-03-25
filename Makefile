run: main.cpp game.h player.h utils.h menu.h settings.h map.h raygui.h
	g++ -std=c++17 -o run main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
