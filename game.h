#ifndef GAME_H
#define GAME_H

struct Bullet {
	int x; 
	int y;
	int speedX;
	int speedY;
};

class Player {
	private:
	public:
		int accelX              = 0;
		int accelY              = 0;
		int velocityX           = 0;
		int velocityY           = 0;
		const int accel_speed   = 5;
		const int max_accel     = 5;
		const int max_velocity  = 20;
		const int max_jump      = 200;
		const int mass          = 3;
		int x, y, width, height;
		Player(int _x, int _y, int _width, int _height){
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
				player.accelX += player.accel_speed;
			}
			if (IsKeyDown(KEY_A)){
				player.accelX -= player.accel_speed;
			}
			if (IsKeyPressed(KEY_W)){
				player.accelY -= 10*player.accel_speed;
				grounded = false;
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
				int temp_x = -camera.offset.x + GetMouseX();
				int temp_y = -camera.offset.y + GetMouseY();

				Rectangle temp = { temp_x, temp_y, 50, 50 };
				objects.push_back(temp);
			}

			// Spawn bullet object
			if(IsMouseButtonPressed(1)){
				int temp_x = -camera.offset.x + GetMouseX();
				int temp_y = -camera.offset.y + GetMouseY();
				int speedX = (temp_x - player.x)/50;
				int speedY = (temp_y - player.y)/50;

				// Fraction speed
				int fracSpeed = 25/(abs(speedX) + abs(speedY));
				speedX *= fracSpeed;
				speedY *= fracSpeed;

				Bullet bullet = {player.x, player.y, speedX + player.velocityX, speedY + player.velocityY};
				bullets.push_back(bullet);

				if(bullets.size() > 100){
					bullets.erase(bullets.begin(), bullets.begin()+1);
				}
			}
		}

		void handlePhysics(Camera2D& camera, Player& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, bool& grounded){
			// PHYSICS
			// Handle player movement




			player.velocityX < -player.max_velocity ? player.velocityX = -player.max_velocity : NULL;
			player.velocityX > player.max_velocity  ? player.velocityX =  player.max_velocity : NULL;

			player.velocityY > player.max_jump  ? player.velocityY = player.max_jump : NULL;
			player.velocityY < -player.max_jump ? player.velocityY = -player.max_jump : NULL;

			player.velocityX += player.accelX;
			player.velocityY += player.accelY;

			player.x += player.velocityX;
			player.y += player.velocityY;

			player.accelX = 0;
			player.accelY = player.mass;

			player.velocityX = floorf(player.velocityX * 100) / 100;
			player.velocityY = floorf(player.velocityY * 100) / 100;

			
			// Friction, wiggly jiggly movement of player is because of that
			(player.velocityX < 2 && player.velocityX > -2) ? player.velocityX = 0 : player.velocityX /= 1.2;
			(player.velocityY < 2 && player.velocityY > -2) ? player.velocityY = 0 : player.velocityY /= 1.2;

			// Handle collision
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
						player.velocityY = 0;
					}                       
					if( ind == 1 ){
						player.y = objects[i].y + objects[i].height;
						grounded = true;
						player.velocityY = 0;
					}                       
					if( ind == 2 ){
						player.x = objects[i].x - player.width;
						player.velocityX = 0;
					}
					if( ind == 3 ){
						player.x = objects[i].x + objects[i].width;
						player.velocityX = 0;
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

		void handleDraw(std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, Camera2D& camera, Player& player){
			BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);
			for( int i = 0; i < objects.size(); i ++ ){
				DrawRectangleRec(objects[i], DARKGRAY);
			}
			for( int i = 0; i < bullets.size(); i ++ ){
				DrawRectangle(bullets[i].x, bullets[i].y, 10, 10, DARKGRAY);
			}
			DrawRectangle(player.x, player.y, player.width, player.height, RED);
			EndMode2D();
			EndDrawing();
		}
};
#endif
