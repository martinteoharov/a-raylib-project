run: main.cpp game.h player.h utils.h menu.h map.h raygui.h weapon.h config.cpp sound.cpp
	g++ -std=c++17 -o run main.cpp config.cpp sound.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
