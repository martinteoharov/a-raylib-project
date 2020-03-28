#ifndef PLAYER_H
#define PLAYER_H
#include "config.h"
class Player {
	private:
		// used in animation
		double time             = GetTime();
		double prevTime         = 0;
		const int animSpeed     = 10;
		int headed              = 1;
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

		void drawState(int currFrame, int state){
			int frames, startPos, frame;
			switch(state){
				case 0: // run
					frames = 5;
					startPos = 9;
					frame = currFrame % frames;
					break;
				case 1: // fixed
					frames = 4;
					startPos = 0;
					frame = currFrame % frames;
					break;
				case 2: // jump
					frames = 2;
					startPos = 15;
					frame = currFrame % frames;
					break;
				case 3: // fall
					frames = 2;
					startPos = 18;
					frame = currFrame % frames;
					break;

			}
			DrawTexturePro(texture,
					Rectangle {(frame+startPos)*widthTex, 0, headed*widthTex, heightTex},
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
			int state;
			time = GetTime();
			if(time > prevTime + (1.0/animSpeed)){
				prevTime = time;
				currFrame ++;
			}

			int temp_headed = velocityX;
			if(temp_headed > 0){
				temp_headed = 1; headed = 1;
			}
			if(temp_headed < 0){
				temp_headed = -1; headed = -1;
			}

			if(velocityY < -1){
				state = 2; // jump
			}
			else if(velocityY > 2){
				state = 3; // fall
			}
			else if(temp_headed != 0){
				state = 0; // run
			}
			else {
				state = 1; // fall
			}

			drawState(currFrame, state);
		}

		void handleKeyPresses(){
			accelX = 0;
			// KEYPRESS
			if (IsKeyDown(KEY_D)){
				accelX = forceX;
			}
			if (IsKeyDown(KEY_A)){
				accelX = -forceX;
			}
			if (IsKeyPressed(KEY_W) && grounded){
				velocityY = -forceY;
				grounded = false;
			}
			if (IsKeyDown(KEY_R)){
				x = config::SCREEN_WIDTH/2;
				y = config::SCREEN_HEIGHT/3;
			}
		}
		void handleMovement(){
			float dt = GetFrameTime() * 60;
			float friction;

			x += float(velocityX);
			y += float(velocityY*dt); //why the fuck?

			velocityX += accelX * dt;
			velocityY += accelY * dt;

			// Apply gravity
			accelY += gravity;
			if(accelY > gravity) accelY = gravity;

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
		void setAccelX(int acc){
			accelX = acc;
		}
		void setAccelY(int acc){
			accelY = acc;
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
