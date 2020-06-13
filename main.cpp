#include <iostream> //cout
#include <vector> //vector
#include <map>
//#include <list>
#include <math.h> //trigonometry
#include <fstream> // read / write
#include <filesystem> // read directories
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include "utils.h"
#include "config.h"
#include "sound.h"
#include "map.h"
#include "bullet.h"
#include "player.h"
#include "weapon.h"
#include "menu.h"
#include "game.h"


int main() {
	
	if(config::LOAD_CONFIG("config/default.conf")){
		std::cout << "CONFIG LOADED SUCCESSFULY" << std::endl;
	}

	InitWindow(config::SCREEN_WIDTH, config::SCREEN_HEIGHT, "a-raylib-project");

	Texture2D texture = LoadTexture("./resources/textures/adventurer.png");
	//            pos X    pos Y         width of texture          height of texture         width                    height                 texture
	Player player(100,     400,          float( texture.width/21), float(texture.height),    config::PLAYER_WIDTH, config::PLAYER_HEIGHT, texture);



	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 0.0f };

	camera.zoom = 1.0f;
	SetTargetFPS(config::FPS);
	InitAudioDevice();      // Initialize audio device
	GuiLoadStyle(("styles/" + config::TEXT_STYLE).c_str());

	if(sound::LOAD_SOUND()){
		std::cout << "SOUND CONFIG LOADED SUCCESSFULY" << std::endl;
	}
	SetMasterVolume(100.0); 



	Menu menu;
	Game game(1, menu);


	std::map<int, std::vector<Rectangle>> mObjects;
	std::vector<Weapon> weapons;
	std::vector<Bullet> bullets;


	//game.getState() 0 - game should close, 1 - menu, 2 - gameplay
	while (game.getState() != 0){ // untill game should close
		bool showGame = false;
		if(game.getState() == 1) {
			std::string menuState = menu.getState();
			
			// PARENT MAIN
			if(menuState == "main"){
				menu.Main();
			}
			else if( menuState == "main-newgame" ){
				std::string fn = menu.mainNewgame();
				if(fn != ""){
					mObjects = Map::serializeRead(fn, player);
				}
			}
			else if( menuState == "main-createmap" ){
				game.setState(3);
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
			else if( menuState == "ingame-settings" ){
				menu.InGameSettings();
			}
			else if( menuState == "ingame-continue" ){
				game.setState(2);
			}
			else if(menuState == "ingame-save"){
				showGame = true;
				std::string fn = menu.InGameSave();
				if(fn != "")
					Map::serializeWrite(mObjects, player, "maps/" + fn);
			}
			else if(menuState == "ingame-quit"){
				game.setState(0);
			}
			menu.handleDraw(camera, showGame, mObjects, bullets, player);
		}
		else if(game.getState() == 2) {
			if(IsKeyDown(KEY_ESCAPE)){
				menu.setState("ingame");
				game.setState(1);
			}

			game.handleKeyPresses(camera, player, mObjects, bullets);
			game.handleMovement(camera, player, mObjects, bullets);
			game.handleCollision(camera, player, mObjects, bullets);
			game.handleDraw(camera, player, mObjects, bullets);

		}
		else if(game.getState() == 3) {
			if(IsKeyDown(KEY_ESCAPE)){
				menu.setState("ingame");
				game.setState(1);
			}

			game.handleKeyPresses(camera, player, mObjects, bullets);
			game.handleMovement(camera, player, mObjects, bullets);
			game.handleCollision(camera, player, mObjects, bullets);
			game.handleDraw(camera, player, mObjects, bullets);

		}
	}
	std::cout << "AVERAGE FPS: " << getAverageFPS() << std::endl;
	CloseWindow();

	return 0;
}
