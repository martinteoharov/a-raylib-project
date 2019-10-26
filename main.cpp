#include <iostream>
#include <vector>
#include <math.h>
#include "raylib.h"
#define MAX_BUILDINGS   10

void handleKeyPress(Camera2D& camera, Rectangle& player, int speed, bool grounded){
	if (IsKeyDown(KEY_D)){
		player.x += speed*GetFrameTime();
	//	camera.offset.x -= speed*GetFrameTime();
	}
	else if (IsKeyDown(KEY_A)){
		player.x -= speed*GetFrameTime();
	//	camera.offset.x += speed*GetFrameTime();
	}
	if (IsKeyDown(KEY_W)){
		player.y -= speed*GetFrameTime();
	}
	if (IsKeyDown(KEY_S)){
		player.y += speed*GetFrameTime();
	}
	if (IsKeyDown(KEY_R)){
		player.x = 1000;
		player.y = 500;
	}
}
void handlePhysics(Camera2D& camera, Rectangle& player, int speed, std::vector<Rectangle> objects, const int gravity, bool& grounded ){
	grounded = false;
	player.y += gravity*GetFrameTime();
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
}
void genBuildings(std::vector<Rectangle>& objects, int buildings, int spacing, const int y, int a, int b){
	for( int i = 0; i < buildings; i ++ ){
		Rectangle obj = { i*spacing + 1500, y, a, b };
		objects.push_back(obj);
	}
}

int main() {
	const int width     = 1920;
	const int height    = 1080;
	const int speed     = 1000;
	const int gravity   = 350;
	bool      grounded  = false;

	InitWindow(width, height, "a-raylib-project");

	Rectangle player = { width/2, height/3, 40, 40 };
	Rectangle floor  = { -100, height/2 + 40, 10000, 50 };

	std::vector<Rectangle> objects;
	objects.push_back(floor);

	genBuildings(objects, 100, 500, 300, 100, 100);

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 2.0f };
	camera.zoom = 1.0f;

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){
		handleKeyPress(camera, player, speed, grounded);
		handlePhysics(camera, player, speed, objects, gravity, grounded);

		std::cout << GetMouseX() << " - " << GetMouseY() << std::endl;
		camera.offset.x = -player.x - GetMouseX()/5 + width/2;
		camera.offset.y = -player.y - GetMouseY()/5 + height/1.5;


		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		//Draw floor
		DrawRectangleRec(floor, DARKGRAY);
		for( int i = 0; i < objects.size(); i ++ ){
			DrawRectangleRec(objects[i], DARKGRAY);
		}
		DrawRectangleRec(player, RED);

		EndMode2D();
		EndDrawing();
	}
	CloseWindow();        // Close window and OpenGL context

	return 0;
}
