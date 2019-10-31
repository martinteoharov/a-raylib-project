#include <iostream>
#include <vector>
#include <math.h>
#include "raylib.h"

#define WIDTH    1920
#define HEIGHT   1080
#define SPEED    1000
#define GRAVITY  0


struct Bullet {
	float x;
	float y;
	float speedX;
	float speedY;
};


//TODO: rewrite this to use new spawnRect function
void genBuildings(std::vector<Rectangle>& objects, int buildings, int spacing, const int y, int a, int b){
	for( int i = 0; i < buildings; i ++ ){
		Rectangle obj = { i*spacing + 1500, y, a, b };
		objects.push_back(obj);
	}
}

void spawnRect(std::vector<Rectangle>& objects, float x, float y, int a, int b){
	Rectangle temp = { x, y, a, b };
	objects.push_back(temp);
}

void spawnBullet(std::vector<Bullet>& bullets, Rectangle& player, float x, float y, float speedX, float speedY){
	Bullet bullet = {x, y, speedX, speedY};
	bullets.push_back(bullet);
}

void handleKeyPress(Camera2D& camera, Rectangle& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, bool grounded){
	if (IsKeyDown(KEY_D)){
		player.x += SPEED*GetFrameTime();
	//	camera.offset.x -= SPEED*GetFrameTime();
	}
	else if (IsKeyDown(KEY_A)){
		player.x -= SPEED*GetFrameTime();
	//	camera.offset.x += SPEED*GetFrameTime();
	}
	if (IsKeyDown(KEY_W)){
		player.y -= SPEED*GetFrameTime();
	}
	if (IsKeyDown(KEY_S)){
		player.y += SPEED*GetFrameTime();
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

		spawnRect(objects, temp_x - 25, temp_y - 25, 50, 50);
	}

	// Spawn bullet object
	if(IsMouseButtonDown(1)){
		float temp_x = -camera.offset.x + GetMouseX();
		float temp_y = -camera.offset.y + GetMouseY();
		float speedX = (temp_x - player.x)/50;
		float speedY = (temp_y - player.y)/50;

		spawnBullet(bullets, player, player.x + player.width/2, player.y + player.height/2, speedX, speedY);

		if(bullets.size() > 100){
			bullets.erase(bullets.begin(), bullets.begin()+1);
		}
	}
	
}
void handlePhysics(Camera2D& camera, Rectangle& player, std::vector<Rectangle>& objects, std::vector<Bullet>& bullets, bool& grounded ){
	grounded = false;
	// Handle gravity, collision
	player.y += GRAVITY*GetFrameTime();
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

	// TODO handle bullets
	for( int i = 0; i < bullets.size(); i ++ ){
		bullets[i].x += bullets[i].speedX;
		bullets[i].y += bullets[i].speedY;
		for( int m = 0; m < objects.size(); m ++ ){
			if( bullets[i].x > objects[m].x                                 &&
					bullets[i].x < objects[m].x + objects[m].width  &&
					bullets[i].y > objects[m].y                     &&
					bullets[i].y < objects[m].y + objects[m].height ){

				bullets.erase(bullets.begin() + i);
			}
		}
	}
	
}

int main() {
	bool grounded  = false;

	InitWindow(WIDTH, HEIGHT, "a-raylib-project");

	Rectangle player = { WIDTH/2, HEIGHT/3, 40, 40 };
	Rectangle floor  = { -100, HEIGHT/2 + 40, 10000, 50 };

	std::vector<Rectangle> objects;
	std::vector<Bullet>    bullets;

	objects.push_back(floor);

	genBuildings(objects, 100, 500, 300, 100, 100);

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 2.0f };
	camera.zoom = 1.0f;

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){
		handleKeyPress(camera, player, objects, bullets, grounded);
		handlePhysics(camera, player, objects, bullets, grounded);

		// Follow player with camera
		camera.offset.x = -player.x - GetMouseX()/5 + WIDTH/2;
		camera.offset.y = -player.y - GetMouseY()/5 + HEIGHT/1.5;

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		DrawRectangleRec(floor, DARKGRAY);
		for( int i = 0; i < objects.size(); i ++ ){
			DrawRectangleRec(objects[i], DARKGRAY);
		}
		for( int i = 0; i < bullets.size(); i ++ ){
			DrawRectangle(bullets[i].x, bullets[i].y, 10, 10, DARKGRAY);
		}
		DrawRectangleRec(player, RED);

		EndMode2D();
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
