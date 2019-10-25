#include <iostream>
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
	if (IsKeyDown(KEY_UP)){
		player.y -= speed*GetFrameTime();
		camera.offset.y += speed*GetFrameTime();
	}
	else if (IsKeyDown(KEY_DOWN)){
		player.y += speed*GetFrameTime();
		camera.offset.y -= speed*GetFrameTime();
	}
}

int main() {
	const int WIDTH     = 1920;
	const int HEIGHT    = 1080;
	const int speed     = 1000;

	InitWindow(WIDTH, HEIGHT, "a-raylib-project");

	Rectangle player = { WIDTH/2, HEIGHT/2, 40, 40 };
	Rectangle buildings[MAX_BUILDINGS] = { 0 };
	Color buildColors[MAX_BUILDINGS] = { 0 };

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 2.0f };
	camera.zoom = 0.8f;

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){
		handleKeyPress(camera, player, speed);

		camera.target.x = player.x;
		camera.target.y = player.y;

		if (IsKeyDown(KEY_A)) camera.rotation--;
		else if (IsKeyDown(KEY_S)) camera.rotation++;

		if (camera.rotation > 40) camera.rotation = 40;
		else if (camera.rotation < -40) camera.rotation = -40;

		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		//Draw floor
		DrawRectangle(0, HEIGHT/2 + 40, 1000, 50, DARKGRAY);
		DrawRectangleRec(player, RED);
		EndMode2D();
		EndDrawing();
	}
	CloseWindow();        // Close window and OpenGL context

	return 0;
}
