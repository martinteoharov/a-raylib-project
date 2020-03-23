#include "constants.h"

#ifndef PLAYER_H
#define PLAYER_H
class Player {
	private:
		// used in animation
		double time             = GetTime();
		double prevTime         = 0;
		const int animSpeed     = 10;
		int prevHeaded          = 1;
		int currFrame           = 0;

		// character velocity
		bool grounded             = false;
		float accelX              = 0;
		float accelY              = 0;
		float velocityX           = 0;
		float velocityY           = 0;
		const float forceX        = 200;
		const float forceY        = 20;
		const float gravity       = 1.1;
		const float max_velocity  = 20;

		//higher is lower
		const float frictionX     = 4;
		float x, y, width, height, widthTex, heightTex;

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
		void drawJump(int headed, int currFrame){
			const int frames = 2;
			const int startPos = 14;
			int frame = currFrame % frames;

			DrawTexturePro(texture,
					Rectangle {(frame+startPos)*widthTex, 0, prevHeaded*widthTex, heightTex},
					Rectangle {x, y, width, height},
					Vector2   {0, 0},
					0.0f,
					RAYWHITE);
		}
		void drawFall(int headed, int currFrame){
			const int frames = 2;
			const int startPos = 16;
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
			if(velocityY < -1){
				drawJump(headed, currFrame);
			}
			else if(velocityY > 2){
				drawFall(headed, currFrame);
			}
			else{
				headed != 0 ? drawRun(headed, currFrame) : drawFixed(prevHeaded, currFrame);
			}
		}

		void handleKeyPresses(std::vector<Rectangle>& objects){
			float dt = GetFrameTime() * 60;
			accelX = 0;
			// KEYPRESS
			if (IsKeyDown(KEY_D)){
				accelX = forceX * dt;
			}
			if (IsKeyDown(KEY_A)){
				accelX = -forceX * dt;
			}
			if (IsKeyPressed(KEY_W) && grounded){
				velocityY = -forceY;
				grounded = false;
			}
			if (IsKeyDown(KEY_R)){
				x = WIDTH/2;
				y = HEIGHT/3;
				// Erase pasted objects
				for( int i = 0; i < objects.size(); i ++ ){
					objects.erase(objects.begin() + 1 + 100, objects.begin() + objects.size());
				}
			}
		}
		void handlePhysics(){
			float dt = GetFrameTime() * 60;
			float friction;

			x += int(velocityX);
			y += int(velocityY);

			velocityX += accelX;
			velocityY += accelY;

			// Apply gravity
			accelY += gravity * dt;
			if(accelY > gravity * dt) accelY = gravity * dt;

			//check if velocity has passed max_velocity in either direction and correct for it
			velocityX < -max_velocity*dt ? velocityX = -max_velocity*dt : NULL;
			velocityX > max_velocity*dt  ? velocityX =  max_velocity*dt : NULL;

			// Apply friction && gravity
			(velocityX < 2.0*dt && velocityX > -2.0*dt) ? velocityX = 0 : velocityX -= (velocityX / frictionX) * dt;
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
		int getAccelX(){
			return accelX;
		}
		int getAccelY(){
			return accelY;
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



#endif
