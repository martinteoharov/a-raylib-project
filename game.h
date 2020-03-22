#include "constants.h"
#ifndef GAME_H
#define GAME_H

struct Bullet {
	float x;
	float y;
	float speedX;
	float speedY;
};


class Game {
	private:
	public:
		void handleKeyPresses(Camera2D& camera, Player& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets){

			player.handleKeyPresses(objects);

			// Spawn rect object
			if(IsMouseButtonDown(0)){
				int temp_x = -camera.offset.x/camera.zoom + GetMouseX()/camera.zoom;
				int temp_y = -camera.offset.y/camera.zoom + GetMouseY()/camera.zoom;

				Rectangle temp = { temp_x, temp_y, 50, 50 };
				objects.push_back(temp);
			}

			// Spawn bullet object
			// TODO: Fix trajectory
			if(IsMouseButtonPressed(1)){
				float temp_x = GetMouseX()/camera.zoom - camera.offset.x/camera.zoom;
				float temp_y = GetMouseY()/camera.zoom - camera.offset.y/camera.zoom;

				float speedX = (temp_x - player.getX());
				float speedY = (temp_y - player.getY());

				float norm = sqrt(speedX*speedX + speedY*speedY);
				speedX /= (norm/10.0f);
				speedY /= (norm/10.0f);
				std::cout << speedX << " - " << speedY << std::endl;

				Bullet bullet = {player.getX(), player.getY(), speedX, speedY};
				bullets.push_back(bullet);

				if(bullets.size() > 100){
					bullets.erase(bullets.begin(), bullets.begin()+1);
				}
			}
			camera.zoom += GetMouseWheelMove()/10.0f;
		}

		void handlePhysics(Camera2D& camera, Player& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets){
			// PHYSICS
			// Handle player movement
			player.handlePhysics();

			// Handle collision
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

			// Handle bullets
			for( int i = 0; i < bullets.size(); i ++ ){
				bullets[i].x += bullets[i].speedX;
				bullets[i].y += bullets[i].speedY;
				for( int m = 0; m < objects.size(); m ++ ){
					if( bullets[i].x > objects[m].x                                 &&
							bullets[i].x < objects[m].x + objects[m].width  &&
							bullets[i].y > objects[m].y                     &&
							bullets[i].y < objects[m].y + objects[m].height ){

						bullets.erase(bullets.begin() + i);
						objects.erase(objects.begin() + m);
						i--;
						m--;
					}
				}
			}

			camera.offset.x = (-player.getX() - GetMouseX()/5 + WIDTH/3/camera.zoom)*camera.zoom;
			camera.offset.y = (-player.getY() - GetMouseY()/5 + HEIGHT/2/camera.zoom)*camera.zoom;
		}

		void handleDraw(std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, Camera2D& camera, Player& player){
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);
			player.animation();

			for( int i = 0; i < objects.size(); i ++ ){
				DrawRectangleRec(objects[i], DARKGRAY);
			}
			for( int i = 0; i < bullets.size(); i ++ ){
				DrawRectangle(bullets[i].x, bullets[i].y, 10, 10, DARKGRAY);
			}
			std::string sText = "FPS: " + std::to_string(GetFPS()) + "\n" + "velocityX:" + std::to_string(player.getVelX()) + " velocityY:" + std::to_string(player.getVelY()) + '\n' + "accelX:" + std::to_string(player.getAccelX()) + " accelY:" + std::to_string(player.getAccelY());
			const char *cText = sText.c_str();
			DrawText(cText, -camera.offset.x/camera.zoom + 10, -camera.offset.y/camera.zoom + 10, 20, GREEN);



			//DrawRectangle(player.getX(), player.getY(), player.getW(), player.getH(), RED);
			EndMode2D();
			EndDrawing();
		}
};
#endif
