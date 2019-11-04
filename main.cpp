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
	InitWindow(WIDTH, HEIGHT, "a-raylib-project");

	Game game;
	Texture2D dino = LoadTexture("./resources/textures/dino_walk_normal.png");
	Player player(WIDTH/2, HEIGHT/3, dino.width/8, dino.height, dino);
	Rectangle floor  = { -100, HEIGHT/2 + 40, 10000, 50 };

	std::vector<Rectangle> objects;
	std::vector<Bullet>    bullets;

	objects.push_back(floor);

	genBuildings(objects, 100, 500, 300, 100, 100);

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 2.0f };
	camera.zoom = 1.0f;

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){
		game.handleKeyPresses(camera, player, objects, bullets);
		game.handlePhysics(camera, player, objects, bullets);
		game.handleDraw(objects, bullets, camera, player);
	}
	CloseWindow();

	return 0;
}
