#ifndef GAME_H
#define GAME_H

struct Bullet {
	float x; 
	float y;
	float speedX;
	float speedY;
};

class Player {
	private:
		float velocity            = 0;
		const float accel_speed   = 50;
		const float max_velocity  = 1000;
	public:
		float x, y, width, height;
		Player(float _x, float _y, float _width, float _height){
			x      = _x;
			y      = _y;
			width  = _width;
			height = _height;
		}

};

class Game {
	private:
		//
	public:
		void handleKeyPresses(Camera2D& camera, Player& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, bool& grounded){

			// KEYPRESS
			if (IsKeyDown(KEY_D)){
				player.x += 20;
			}
			else if (IsKeyDown(KEY_A)){
				player.x -= 20;
			}
			if (IsKeyPressed(KEY_W) && grounded){
				player.y -= 100;
				grounded = false;
			}
			if (IsKeyDown(KEY_S)){
				player.y += 20;
			}
			if (IsKeyDown(KEY_R)){
				player.x = 1000;
				player.y = 500; 
				// Erase pasted objects
				for( int i = 0; i < objects.size(); i ++ ){
					objects.erase(objects.begin() + 1 + 100, objects.begin() + objects.size());
				}
			}        
			if (IsKeyDown(KEY_R)){
				player.x = 1000;
				player.y = 500; 
			}

			// Spawn rect object
			if(IsMouseButtonDown(0)){
				float temp_x = -camera.offset.x + GetMouseX();
				float temp_y = -camera.offset.y + GetMouseY();

				Rectangle temp = { temp_x, temp_y, 50, 50 };
				objects.push_back(temp);
			}

			// Spawn bullet object
			if(IsMouseButtonPressed(1)){
				float temp_x = -camera.offset.x + GetMouseX();
				float temp_y = -camera.offset.y + GetMouseY();
				float speedX = (temp_x - player.x)/50;
				float speedY = (temp_y - player.y)/50;

				// Fraction speed
				float fracSpeed = 25/(abs(speedX) + abs(speedY));
				speedX *= fracSpeed;
				speedY *= fracSpeed;

				Bullet bullet = {player.x, player.y, speedX, speedY};
				bullets.push_back(bullet);

				if(bullets.size() > 100){
					bullets.erase(bullets.begin(), bullets.begin()+1);
				}
			}
		}

		void handlePhysics(Camera2D& camera, Player& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, bool& grounded){

			// PHYSICS
			// Handle gravity, collision
			player.y += 2;
			for( int i = 0; i < objects.size(); i ++ ){
				if( player.x > objects[i].x - player.width                  &&
						player.x < objects[i].x + objects[i].width  &&
						player.y > objects[i].y - player.height     &&
						player.y < objects[i].y + objects[i].height ){

					int side[4];
					side[0] = player.y + player.height - objects[i].y;
					side[1] = player.y - objects[i].y - objects[i].height;

					side[2] = player.x - objects[i].x + player.width;
					side[3] = player.x - objects[i].x - objects[i].width;



					int s = 10000;
					int ind;      
					for( int i = 0; i < 4; i ++ ){
						abs(side[i]) < s ? (s = abs(side[i]), ind = i) : NULL;
					}

					if( ind == 0 ){
						player.y = objects[i].y - player.height;
						grounded = true;
					}                       
					if( ind == 1 ){
						player.y = objects[i].y + objects[i].height;
						grounded = true;
					}                       
					if( ind == 2 ){
						player.x = objects[i].x - player.width;
					}
					if( ind == 3 ){
						player.x = objects[i].x + objects[i].width;
					}
				}        
			}        

			// TODO handle bullets better
			for( int i = 0; i < bullets.size(); i ++ ){
				bullets[i].x += bullets[i].speedX;
				bullets[i].y += bullets[i].speedY;
				for( int m = 0; m < objects.size(); m ++ ){
					if( bullets[i].x > objects[m].x                                 &&
							bullets[i].x < objects[m].x + objects[m].width  &&
							bullets[i].y > objects[m].y                     &&
							bullets[i].y < objects[m].y + objects[m].height ){

						bullets.erase(bullets.begin() + i);
						i--;
					}
				}
			}
			camera.offset.x = -player.x - GetMouseX()/5 + 1920/2;
			camera.offset.y = -player.y - GetMouseY()/5 + 1080/1.5;
		}


};
#endif
