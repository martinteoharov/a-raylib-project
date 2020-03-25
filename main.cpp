//cout
#include <iostream>
//vector
#include <vector>
//trigonometry
#include <math.h>
#include <fstream> // read / write

#include <filesystem> // read directories


#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"

#include "utils.h"
#include "settings.h"
#include "map.h"
#include "bullet.h"
#include "player.h"
#include "menu.h"
#include "game.h"


int main() {
	InitWindow(WIDTH, HEIGHT, "a-raylib-project");

	Texture2D texture = LoadTexture("./resources/textures/adventurer.png");
	//            pos X    pos Y         width of texture          height of texture         width                    height                 texture
	Player player(100,     400,          float( texture.width/21), float(texture.height),    float(texture.width/10), float(texture.height*2), texture);


	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 0.0f };
	camera.zoom = 1.0f;
	SetTargetFPS(60);

	Menu menu;
	Game game(1, menu);

	std::vector<Rectangle> objects;
	std::vector<Bullet>    bullets;

	while (game.getState() != 0){ // untill game should close
		bool showGame = false;
		if(game.getState() == 1) {
			std::string menuState = menu.getState();
			
			// PARENT MAIN
			if(menuState == "main"){
				menu.Main();
			}
			else if( menuState == "main-newgame" ){
				objects = Map::serializeRead("maps/static/default.map");
				game.setState(2);
			}
			else if( menuState == "main-loadgame" ){
				std::string fn = menu.mainLoadgame();
				if(fn != "")
					objects = Map::serializeRead(fn);
			}
			else if( menuState == "main-settings" ){
				menu.mainSettings();
			}
			else if( menuState == "main-quit" ){
				game.setState(0);
	        	}


			// PARENT INGAME
			else if(menuState == "ingame"){
				menu.InGame();
				showGame = true;
			}
			else if( menuState == "ingame-continue" ){
				game.setState(2);
			}
			else if(menuState == "ingame-save"){
				std::string fn = menu.InGameSave();
				if(fn != "")
					Map::serializeWrite(objects, "maps/" + fn);
			}
			else if(menuState == "ingame-quit"){
				game.setState(0);
			}
			menu.handleDraw(camera, showGame, objects, bullets, player);
		}
		else if(game.getState() == 2) {
			if(IsKeyDown(KEY_ESCAPE)){
				menu.setState("ingame");
				game.setState(1);
			}

			game.handleKeyPresses(camera, player, objects, bullets);
			game.handlePhysics(camera, player, objects, bullets);
			game.handleDraw(objects, bullets, camera, player);

		}
	}
	CloseWindow();

	return 0;
}
