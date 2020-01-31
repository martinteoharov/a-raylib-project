#ifndef GAME_H
#define GAME_H

struct Bullet {
	int x;
	int y;
	int speedX;
	int speedY;
};

// Rewrite handle movement function to be here
class Player {
	private:
		// used in animation
		double time     = GetTime();
		double prevTime         = 0;
		int prevHeaded          = 1;
		int animSpeed           = 10;
		int currFrame           = 0;

		// character velocity
		bool grounded           = false;
		float accelX              = 0;
		float accelY              = 0;
		float velocityX           = 0;
		float velocityY           = 0;
		const float accel_speed   = 5;
		const float max_accel     = 10;
		const float max_velocity  = 15;
		const float max_jump      = 600;
		const float mass          = 2.5;
		const float frictionX     = 1.4;
		const float frictionY     = 1.2;
		int x, y, width, height, widthTex, heightTex;

		
		// Texture & Animation
		Texture2D texture;

		void drawRun(int headed, int currFrame){
			const int frames = 5;
			const int startPos = 9;
			int frame = currFrame % frames;

			DrawTexturePro(texture,
					Rectangle {(frame+startPos)*widthTex, 0, headed*widthTex, heightTex},
					Rectangle {x, y, width, height},
					Vector2   {0, 0},
					0.0f,
					RAYWHITE);
		}
		void drawFixed(int headed, int currFrame){
			const int frames = 4;
			const int startPos = 0;
			int frame = currFrame % frames;

			DrawTexturePro(texture,
					Rectangle {(frame+startPos)*widthTex, 0, prevHeaded*widthTex, heightTex},
					Rectangle {x, y, width, height},
					Vector2   {0, 0},
					0.0f,
					RAYWHITE);

		}
	public:
		Player(int _x, int _y, int _widthTex, int _heightTex, int _width, int _height, Texture2D _player){
			x         = _x;
			y         = _y;
			widthTex  = _widthTex;
			heightTex = _heightTex;
			width     = _width;
			height    = _height;
			texture   = _player;
		}
		void animation(){
			time = GetTime();
			if(time > prevTime + (1.0/animSpeed)){
				prevTime = time;
				currFrame ++;
			}
			int headed = velocityX;
			if(headed > 0) {headed = 1; prevHeaded = 1;}
			if(headed < 0) {headed = -1; prevHeaded = -1;}
			headed != 0 ? drawRun(headed, currFrame) : drawFixed(prevHeaded, currFrame);
		}

		void handleKeyPresses(std::vector<Rectangle>& objects){
			// KEYPRESS
			if (IsKeyDown(KEY_D)){
				accelX += accel_speed;
			}
			if (IsKeyDown(KEY_A)){
				accelX -= accel_speed;
			}
			if (IsKeyPressed(KEY_W) && grounded){
				accelY -= 10*accel_speed;
				grounded = false;
			}
			if (IsKeyDown(KEY_R)){
				x = 1000;
				y = 500;
				// Erase pasted objects
				for( int i = 0; i < objects.size(); i ++ ){
					objects.erase(objects.begin() + 1 + 100, objects.begin() + objects.size());
				}
			}
			if (IsKeyDown(KEY_R)){
				x = 1000;
				y = 500;
			}
		}
		void handlePhysics(){
			velocityX < -max_velocity ? velocityX = -max_velocity : NULL;
			velocityX > max_velocity  ? velocityX =  max_velocity : NULL;

			velocityY > max_jump  ? velocityY =  max_jump : NULL;
			velocityY < -max_jump ? velocityY = -max_jump : NULL;

			velocityX += accelX;
			velocityY += accelY;

			x += velocityX;
			y += velocityY;

			// Apply gravity
			accelX = 0;
			accelY = mass;

			// Apply friction
			(velocityX < 2 && velocityX > -2) ? velocityX = 0 : velocityX /= frictionX;
			(velocityY < 2 && velocityY > -2) ? velocityY = 0 : velocityY /= frictionY;
		}
		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
		int getW(){
			return width;
		}
		int getH(){
			return height;
		}
		int getVelX(){
			return velocityX;
		}
		int getVelY(){
			return velocityY;
		}
		void setVelX(int vel){
			velocityX = vel;
		}
		void setVelY(int vel){
			velocityY = vel;
		}
		void setX(int _x){
			x = _x;
		}
		void setY(int _y){
			y = _y;
		}
		void setGrounded(bool _grounded){
			grounded = _grounded;
		}

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

			camera.offset.x = (-player.getX() - GetMouseX()/5 + 1920/3/camera.zoom)*camera.zoom;
			camera.offset.y = (-player.getY() - GetMouseY()/5 + 1080/2/camera.zoom)*camera.zoom;
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
			DrawFPS(-camera.offset.x/camera.zoom + 10, -camera.offset.y/camera.zoom + 10);


			//DrawRectangle(player.getX(), player.getY(), player.getW(), player.getH(), RED);
			EndMode2D();
			EndDrawing();
		}
};
#endif
