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
#endif
