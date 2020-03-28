#ifndef GAME_H
#define GAME_H
#include "config.h"

class Game {
	private:
		int currFrame = 1;
		int fps;
		int state; // 0 - should close, 1 - main menu, 2 - gameplay
		std::string sText;
		Menu menu;
	public:
		Game(int _state, Menu _menu){
			state = _state;
			menu = _menu;
		}
		void handleKeyPresses(Camera2D& camera, Player& player, std::map<int, std::vector<Rectangle>>& mObjects, std::vector<Bullet>& bullets){

			player.handleKeyPresses();

			// Spawn rect object
			if(IsMouseButtonDown(0)){
				std::cout << "New element" << std::endl;
				int temp_x = -camera.offset.x/camera.zoom + GetMouseX()/camera.zoom;
				int temp_y = -camera.offset.y/camera.zoom + GetMouseY()/camera.zoom;

				int norm_x = temp_x / GRID_SIZE;

				Rectangle temp = { temp_x, temp_y, 50, 50 };
				Map::createRect(temp, mObjects);
			}

			// Spawn bullet object
			// TODO: Fix trajectory
			if(IsMouseButtonPressed(1)){
				float temp_x = GetMouseX()/camera.zoom - camera.offset.x/camera.zoom;
				float temp_y = GetMouseY()/camera.zoom - camera.offset.y/camera.zoom;

				float speedX = (temp_x - player.getX());
				float speedY = (temp_y - player.getY());

				float norm = sqrt(speedX*speedX + speedY*speedY);

				speedX /= (norm/1000.0f); //1000.0f here is the speed of the bullet
				speedY /= (norm/1000.0f);

				Bullet bullet = {player.getX(), player.getY(), speedX, speedY};
				bullets.push_back(bullet);

				//Push Back
				//player.setVelX(-speedX);
				//player.setVelY(-speedY);

				if(bullets.size() > 100){
					bullets.erase(bullets.begin(), bullets.begin()+1);
				}
			}
			if(IsKeyDown(KEY_ESCAPE)){
			       	state = 1;
			}
			camera.zoom += GetMouseWheelMove()/10.0f;
		}

		void handleMovement(Camera2D& camera, Player& player, std::map<int, std::vector<Rectangle>>& mObjects, std::vector<Bullet>& bullets){
			// PHYSICS
			// Handle player movement
			player.handleMovement();
			
			// Handle bullets
			for( int i = 0; i < bullets.size(); i ++ ){
				const float dt = GetFrameTime();

				bullets[i].x += bullets[i].speedX * dt;
				bullets[i].y += bullets[i].speedY * dt;

				int norm_x = bullets[i].x / GRID_SIZE;

				std::vector<Rectangle> objects = mObjects[norm_x];

				for( int m = 0; m < objects.size(); m ++ ){
					if( bullets[i].x > objects[m].x                                 &&
							bullets[i].x < objects[m].x + objects[m].width  &&
							bullets[i].y > objects[m].y                     &&
							bullets[i].y < objects[m].y + objects[m].height ){

						bullets.erase(bullets.begin() + i);
						objects.erase(objects.begin() + m);
						mObjects[norm_x] = objects;
						i--;
						m--;
					}
				}
			}

			camera.offset.x = (-player.getX() - GetMouseX()/5 + config::SCREEN_WIDTH/3/camera.zoom)*camera.zoom;
			camera.offset.y = (-player.getY() - GetMouseY()/5 + config::SCREEN_HEIGHT/2/camera.zoom)*camera.zoom;
		}
		void handleCollision(Camera2D& camera, Player& player, std::map<int, std::vector<Rectangle>>& mObjects, std::vector<Bullet>& bullets){
			// Handle collision
			int normX = (player.getX() + player.getW()) / GRID_SIZE;
			std::vector<Rectangle> objects = mObjects[normX];

			for( int i = 0; i < objects.size(); i ++ ){
				if( player.getX() > objects[i].x - player.getW()                 &&
						player.getX() < objects[i].x + objects[i].width  &&
						player.getY() > objects[i].y - player.getH()     &&
						player.getY() < objects[i].y + objects[i].height ){

					int side[4];
					side[0] = player.getY() + player.getH() - objects[i].y;
					side[1] = player.getY() - objects[i].y - objects[i].height;

					side[2] = player.getX() - objects[i].x + player.getW();
					side[3] = player.getX() - objects[i].x - objects[i].width;

					int s = 10000;
					int ind;
					// Handle obj collisions
					for( int i = 0; i < 4; i ++ ){
						abs(side[i]) < s ? (s = abs(side[i]), ind = i) : NULL;
					}
					switch (ind){
						case 0:
							player.setY(objects[i].y - player.getH());
							player.setGrounded(true);
							player.setVelY(0);
							break;
						case 1:
							player.setY(objects[i].y + objects[i].height);
							player.setGrounded(true);
							player.setVelY(0);
							break;
						case 2:
							player.setX(objects[i].x - player.getW());
							player.setVelX(0);
							break;
						case 3:
							player.setX(objects[i].x + objects[i].width);
							player.setVelX(0);
							break;
					}
				}
			}
		}

		void handleDraw(std::map<int,std::vector<Rectangle>>& mObjects, std::vector<Bullet>& bullets, Camera2D& camera, Player& player){
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);
			player.animation();
			getAverageFPS();

			int norm_x = player.getX() / GRID_SIZE;

			//draw 2 chunks
			std::vector<Rectangle> objects  = mObjects[norm_x];
			std::vector<Rectangle> objects2 = mObjects[norm_x + 1];

			objects.insert( objects.end(), objects2.begin(), objects2.end() );


			for( int i = 0; i < objects.size(); i ++ ){
				DrawRectangleRec(objects[i], DARKGRAY);
			}
			for( int i = 0; i < bullets.size(); i ++ ){
				DrawRectangle(bullets[i].x, bullets[i].y, 10, 10, DARKGRAY);
			}


			DrawFPS(-camera.offset.x/camera.zoom + 10, -camera.offset.y/camera.zoom + 10);
			if(GetFrameTime() * currFrame > 0.1){
				sText = "velocityX:" + std::to_string(player.getVelX()) + " velocityY:" + std::to_string(player.getVelY()) + '\n' + "accelX:" + std::to_string(player.getAccelX()) + " accelY:" + std::to_string(player.getAccelY());
				currFrame = 0;
			}
			const char *cText = sText.c_str();
			DrawText(cText, -camera.offset.x/camera.zoom + 10, -camera.offset.y/camera.zoom + 30, 20, DARKGREEN);

			currFrame ++;



			//DrawRectangle(player.getX(), player.getY(), player.getW(), player.getH(), RED);
			EndMode2D();
			EndDrawing();
		}
		void Init(){

		}
		int getState(){
			return state;
		}
		int setState(int _state){
			state = _state;
		}
};
#endif
