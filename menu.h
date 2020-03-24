#ifndef MENU_H
#define MENU_H

class Menu {
	private:
	public:
		int main(){
			const int btnWidth = 200;
			const int btnHeight = 50;
			const int marginTop = btnHeight + 10;
			std::string titles [] = {"New Game", "Load Game", "Settings", "Exit"};

			for(int i = 0; i < sizeof(titles)/sizeof(std::string); i ++ ){
				if(GuiButton({WIDTH/2 - btnWidth/2, HEIGHT/4 + i * marginTop, btnWidth, btnHeight}, titles[i].c_str()) == 1){
					if( i == 0 ){
						return 2; // gameplay
					}
					if( i == 3 ){
						return 0;
					}
				}
			}
			return 1;
		}
		void handleDraw(Camera2D& camera){
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);


			EndMode2D();
			EndDrawing();
		}

};
#endif
