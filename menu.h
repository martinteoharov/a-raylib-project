#ifndef MENU_H
#define MENU_H
#include "config.h"

class Menu {
	private:
		char fn[200] = "";
		float value = config::FPS;
		std::string state = "main";

		// used in mainSettings
		int active = 0;
		int oldActive;
	public:
		void Main(){
			state = "main";

			const int btnWidth = 200;
			const int btnHeight = 50;
			const int marginTop = btnHeight + 10;
			std::string titles [] = {"New Game", "Create Map", "Settings", "Quit"};

			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({config::SCREEN_WIDTH/2 - btnWidth/2, config::SCREEN_HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0: // new game
							state = "main-newgame";
							break;
						case 1:
							state = "main-createmap";
							break;
						case 2:
							state = "main-settings";
							break;
						case 3: // exit
							state = "main-quit";
							break;
					}
				}
			}
		}
		void InGame(){
			state = "ingame";
			
			const int btnWidth = 200;
			const int btnHeight = 50;
			const int marginTop = btnHeight + 10;
			std::string titles [] = {"Resume", "Save", "Settings", "Exit"};

			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({config::SCREEN_WIDTH/2 - btnWidth/2, config::SCREEN_HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0:
							state = "ingame-continue";
							break;
						case 1:
							state = "ingame-save"; // go to save
							break;
						case 2:
							state = "ingame-settings"; // go back to main
							break;
						case 3:
							state = "main"; // go back to main
							break;
					}
				}
			}
		}
		std::string InGameSave(){
			state = "ingame-save";
			
			const int boxWidth = 500;
			const int boxHeight = 200;


			int pressed = GuiTextInputBox({config::SCREEN_WIDTH/2 - boxWidth/2, config::SCREEN_HEIGHT/2 - boxHeight/2, boxWidth, boxHeight}, "Save as:", "Example: filename.map", "Save", fn);
			if(pressed == 1){
				state = "ingame-continue";
				return fn;
			}
			if(pressed == 0){
				state = "ingame";
			}

			return "";
		}
		void InGameSettings(){
			state = "ingame-settings";
			
			const int btnWidth = 200;
			const int btnHeight = 50;
			const int sliderWidth = 200;
			const int sliderHeight = 20;
			const int comboBoxHeight = 50;
			const int comboBoxWidth  = 200;
			const int marginTop = btnHeight + 10;

			char fps_buf[64];
			int ret = snprintf(fps_buf, sizeof fps_buf, "%f", value);

			value = GuiSlider({config::SCREEN_WIDTH/2 - sliderWidth/2, 150, sliderWidth, sliderHeight}, "FPS Target", fps_buf, value, 0, 1000);


			const char *pixelFormatTextList[2] = { "CHERRY.RGS", "CANDY.RGS" };
			active = GuiComboBox({config::SCREEN_WIDTH/2 - comboBoxWidth/2, 50, comboBoxWidth, comboBoxHeight}, TextJoin(pixelFormatTextList, 2, ";"), active);
			if(active != oldActive){
				std::cout << pixelFormatTextList[active] << std::endl;
				std::string str(pixelFormatTextList[active], strnlen(pixelFormatTextList[active], 100));
				GuiLoadStyle(("styles/" + str).c_str());
			}
			oldActive = active;

			std::string titles [] = {"Save", "Resume"};

			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({config::SCREEN_WIDTH/2 - btnWidth/2, config::SCREEN_HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0:
							config::FPS = value;
							SetTargetFPS(config::FPS);
							state = "ingame-continue"; // go back to main
							break;
						case 1:
							value = config::FPS;
							state = "ingame-continue"; // go back to main
							break;
					}
				}
			}

		}
		void mainSettings(){
			state = "main-settings";
			
			const int btnWidth       = 200;
			const int btnHeight      = 50;
			const int sliderWidth    = 200;
			const int sliderHeight   = 20;
			const int comboBoxHeight = 50;
			const int comboBoxWidth  = 200;
			const int marginTop = btnHeight + 10;

			std::string titles [] = {"Save", "Back"};


			char fps_buf[64];
			int ret = snprintf(fps_buf, sizeof fps_buf, "%f", value);

			value = GuiSlider({config::SCREEN_WIDTH/2 - sliderWidth/2, 150, sliderWidth, sliderHeight}, "FPS Target", fps_buf, value, 0, 1000);


			const char *pixelFormatTextList[2] = { "CHERRY.RGS", "CANDY.RGS" };
			active = GuiComboBox({config::SCREEN_WIDTH/2 - comboBoxWidth/2, 50, comboBoxWidth, comboBoxHeight}, TextJoin(pixelFormatTextList, 2, ";"), active);
			if(active != oldActive){
				std::cout << pixelFormatTextList[active] << std::endl;
				std::string str(pixelFormatTextList[active], strnlen(pixelFormatTextList[active], 100));
				GuiLoadStyle(("styles/" + str).c_str());
			}
			oldActive = active;


			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({config::SCREEN_WIDTH/2 - btnWidth/2, config::SCREEN_HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					switch (i){
						case 0:
							config::FPS = value;
							SetTargetFPS(config::FPS);
							state = "main"; // go back to main
							break;
						case 1:
							state = "main"; // go back to main
							break;
					}
				}
			}

		}
		std::string mainNewgame(){
			state = "main-newgame";
			
			const int btnWidth = 200;
			const int btnHeight = 50;
			
			const int sliderWidth = 200;
			const int sliderHeight = 20;


			const int marginTop = btnHeight + 10;
			std::string titles [] = {"Save", "Exit"};


			std::vector<std::string> stringvec;
			Map::listMaps("./maps/", stringvec);


			for(int i = 0; i < stringvec.size(); i ++ ){
				if(GuiButton({config::SCREEN_WIDTH/2 - btnWidth/2, config::SCREEN_HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, stringvec[i].c_str()) == 1){
					state = "ingame-continue"; // go back to main
					return stringvec[i].c_str();
				}
			}
			return "";
		}

		void handleDraw(Camera2D& camera, bool showGame, std::map<int, std::vector<Rectangle>> &mObjects, std::vector<Bullet> &bullets, Player& player){
			
			BeginDrawing();
			if(!showGame)
				ClearBackground(RAYWHITE);
			BeginMode2D(camera);

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
