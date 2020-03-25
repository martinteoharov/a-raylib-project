#ifndef MENU_H
#define MENU_H

class Menu {
	private:
		char *fn;
		float value;
		std::string state = "showmain";
	public:
		void showMain(){
			state = "showmain";
			

			const int btnWidth = 200;
			const int btnHeight = 50;
			const int marginTop = btnHeight + 10;
			std::string titles [] = {"New Game", "Load Game", "Settings", "Quit"};

			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({WIDTH/2 - btnWidth/2, HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0: // new game
							state = "newgame";
							break;
						case 1:
							state = "show-loadgame-main";
							break;
						case 2:
							state = "show-settings-main";
							break;
						case 3: // exit
							state = "quit";
							break;
					}
				}
			}
		}
		void showInGameMain(){
			state = "show-ingame-main";
			
			const int btnWidth = 200;
			const int btnHeight = 50;
			const int marginTop = btnHeight + 10;
			std::string titles [] = {"Save", "Exit"};

			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({WIDTH/2 - btnWidth/2, HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0:
							state = "show-ingame-save-main"; // go back to main
							break;
						case 1:
							state = "showmain"; // go back to main
							break;
					}
				}
			}
		}
		std::string showInGameSaveMain(){
			state = "show-ingame-save-main";
			
			const int boxWidth = 500;
			const int boxHeight = 200;

			std::string titles [] = {"Save", "Exit"};

			int pressed = GuiTextInputBox({WIDTH/2 - boxWidth/2, HEIGHT/2 - boxHeight/2, boxWidth, boxHeight}, "Save as:", "Example: filename.map", "Save", fn);
			if(pressed == 1){
				state = "savegame-continue";
				return fn;
			}

			return "";
		}

		void showSettingsMain(){
			state = "show-settings-main";
			
			const int btnWidth = 200;
			const int btnHeight = 50;
			
			const int sliderWidth = 200;
			const int sliderHeight = 20;


			const int marginTop = btnHeight + 10;
			std::string titles [] = {"Save", "Exit"};


			char buffer[64];
			int ret = snprintf(buffer, sizeof buffer, "%f", value);


			value = GuiSlider({WIDTH/2 - sliderWidth/2, 100, sliderWidth, sliderHeight}, "FPS Target", buffer, value, 0, 1000);
			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({WIDTH/2 - btnWidth/2, HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0:
							// do data serialization
							state = "showmain"; // go back to main
							break;
						case 1:
							state = "showmain"; // go back to main
							break;
					}
				}
			}

		}
		std::string showLoadgameMain(){
			state = "show-loadgame-main";
			
			const int btnWidth = 200;
			const int btnHeight = 50;
			
			const int sliderWidth = 200;
			const int sliderHeight = 20;


			const int marginTop = btnHeight + 10;
			std::string titles [] = {"Save", "Exit"};


			std::vector<std::string> stringvec;
			Map::listMaps("./maps/", stringvec);


			for(int i = 0; i < stringvec.size(); i ++ ){
				if(GuiButton({WIDTH/2 - btnWidth/2, HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, stringvec[i].c_str()) == 1){
					switch (i){
						case 0:
							// do data serialization
							state = "loadgame-continue"; // go back to main
							return stringvec[i].c_str();
						case 1:
							state = "loadgame-continue"; // go back to main
							return stringvec[i].c_str();
					}
				}
			}
			return "";
		}

		void handleDraw(Camera2D& camera, bool showGame, std::vector<Rectangle> &objects, std::vector<Bullet> &bullets, Player& player){
			
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);

			if(showGame){
				player.animation();
				for( int i = 0; i < objects.size(); i ++ ){
					DrawRectangleRec(objects[i], DARKGRAY);
				}
				for( int i = 0; i < bullets.size(); i ++ ){
					DrawRectangle(bullets[i].x, bullets[i].y, 10, 10, DARKGRAY);
				}
				std::string sText = "FPS: " + std::to_string(GetFPS()) + "\n" + "velocityX:" + std::to_string(player.getVelX()) + " velocityY:" + std::to_string(player.getVelY()) + '\n' + "accelX:" + std::to_string(player.getAccelX()) + " accelY:" + std::to_string(player.getAccelY());

			}


			EndMode2D();
			EndDrawing();
			
		}
		std::string getState(){
			return state;
		}
		void setState(std::string _state){
			state = _state;
		}

};
#endif
