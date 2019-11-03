#include <iostream>
#include <vector>
#include <math.h>
#include "raylib.h"
#include "game.h"

#define WIDTH    1920
#define HEIGHT   1080
#define SPEED    1000
#define GRAVITY  400


//TODO: rewrite this to use new spawnRect function
void genBuildings(std::vector<Rectangle>& objects, int buildings, int spacing, const int y, int a, int b){
	for( int i = 0; i < buildings; i ++ ){
		Rectangle obj = { i*spacing + 1500, y, a, b };
		objects.push_back(obj);
	}
}

int main() {
	bool grounded  = false;

	InitWindow(WIDTH, HEIGHT, "a-raylib-project");

	Player player(WIDTH/2, HEIGHT/3, 40, 40);
	Rectangle floor  = { -100, HEIGHT/2 + 40, 10000, 50 };

	std::vector<Rectangle> objects;
	std::vector<Bullet>    bullets;

	objects.push_back(floor);

	genBuildings(objects, 100, 500, 300, 100, 100);

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 2.0f };
	camera.zoom = 1.0f;

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){
		player.handle(camera, player, objects, bullets, grounded);

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
		DrawRectangle(player.x, player.y, player.width, player.height, RED);

		EndMode2D();
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
