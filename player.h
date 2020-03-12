#ifndef PLAYER_H
#define PLAYER_H
class Player {
	private:
		// used in animation
		double time             = GetTime();
		double prevTime         = 0;
		double frameTime        = GetFrameTime();
		const int animSpeed     = 10;
		int prevHeaded          = 1;
		int currFrame           = 0;

		// character velocity
		bool grounded             = false;
		float accelX              = 0;
		float accelY              = 0;
		float velocityX           = 0;
		float velocityY           = 0;
		const float accel_speed   = 5;
		const float max_accel     = 10;
		const float max_velocity  = 40;
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
		}
		void handlePhysics(){
			// Apply gravity
			// obsolete for now (because accelX never reaches max_accel) but don't remove
			accelX > max_accel ? accelX = max_accel : NULL;
			accelX < -max_accel ? accelX = -max_accel : NULL;

			// lower val in (accelX /= val) = more slippery
			accelX > 1 || accelX < -1 ? accelX /= 1.5 : accelX = 0;

			velocityX += accelX;
			velocityY += accelY;

			//check if velocity has passed max_velocity in either direction and correct for it
			velocityX < -max_velocity ? velocityX = -max_velocity : NULL;
			velocityX > max_velocity  ? velocityX =  max_velocity : NULL;

			//check if velocity has passed max_velocity in either direction and correct for it
			velocityY > max_jump  ? velocityY =  max_jump : NULL;
			velocityY < -max_jump ? velocityY = -max_jump : NULL;

			// Apply friction && gravity
			(velocityX < 2 && velocityX > -2) ? velocityX = 0 : velocityX /= frictionX;
			(velocityY < 2 && velocityY > -2) ? velocityY = 0 : velocityY /= frictionY;
			accelY = mass;

			x += velocityX;
			y += velocityY;
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
