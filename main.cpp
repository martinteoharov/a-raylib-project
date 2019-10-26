#include <iostream>
#include <vector>
#include <math.h>
#include "raylib.h"
#define MAX_BUILDINGS   10

void handleKeyPress(Camera2D& camera, Rectangle& player, int speed){
	if (IsKeyDown(KEY_RIGHT)){
		player.x += speed*GetFrameTime();
		camera.offset.x -= speed*GetFrameTime();
	}
	else if (IsKeyDown(KEY_LEFT)){
		player.x -= speed*GetFrameTime();
		camera.offset.x += speed*GetFrameTime();
	}
	if (IsKeyDown(KEY_UP) ){
		player.y -= speed*2*GetFrameTime();
	}
}
void handlePhysics(Camera2D& camera, Rectangle& player, int speed, std::vector<Rectangle> objects, const int gravity ){
	for( int i = 0; i < objects.size(); i ++ ){
		if( player.x + player.width > objects[i].x && player.x < objects[i].x + objects[i].width && player.y + player.height > objects[i].y && player.y < objects[i].y + objects[i].height ){
			int side[4];
			side[0] = player.y + player.height - objects[i].y;
			side[1] = player.y - objects[i].y - objects[i].height;

			side[2] = player.x - objects[i].x;
			side[3] = player.x - objects[i].x - objects[i].width;



			int s = 10000;
			int ind;
			for( int i = 0; i < 4; i ++ ){
				abs(side[i]) < s ? (s = abs(side[i]), ind = i) : NULL;
			}
			std::cout << s << " - " << ind << std::endl;
			if( ind == 0 ){
				player.y = objects[i].y - player.height - 1;
			}
			ind == 0 ? player.y = objects[i].y - player.height - 1 : ind == 1 ? player.y = objects[i].y + objects[i].height : ind == 2 ? player.x = objects[i].x - player.width : ind == 3 ? player.x = objects[i].x + objects[i].width : NULL;
		}
		else {
			player.y += gravity*GetFrameTime();
		}
	}
}

int main() {
	const int WIDTH   = 1920;
	const int HEIGHT  = 1080;
	const int speed   = 1000;
	const int gravity = 200;

	InitWindow(WIDTH, HEIGHT, "a-raylib-project");

	Rectangle player = { WIDTH/2, HEIGHT/2, 40, 40 };
	Rectangle floor  = { 0, HEIGHT/2 + 40, 1000, 50 };
	Rectangle obj    = { 100, HEIGHT/2 - 200, 100, 100 };

	std::vector<Rectangle> objects;
	objects.push_back(floor);
	objects.push_back(obj);

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 2.0f };
	camera.zoom = 0.8f;

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){
		handleKeyPress(camera, player, speed);
		handlePhysics(camera, player, speed, objects, gravity);

		camera.target.x = player.x;


		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		//Draw floor
		DrawRectangleRec(floor, DARKGRAY);
		DrawRectangleRec(obj, DARKGRAY);
		DrawRectangleRec(player, RED);
		EndMode2D();
		EndDrawing();
	}
	CloseWindow();        // Close window and OpenGL context

	return 0;
}
