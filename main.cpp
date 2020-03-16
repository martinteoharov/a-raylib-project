//cout
#include <iostream>
//vector
#include <vector>
//trigonometry
#include <math.h>

#include "raylib.h"
#include "utils.h"
#include "player.h"
#include "game.h"
#include "constants.h"

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
	Texture2D texture = LoadTexture("./resources/textures/adventurer.png");
	//            pos X    pos Y     width of texture  height of texture  width             height          texture
	Player player(WIDTH/2, HEIGHT/3, texture.width/21, texture.height,    texture.width/21, texture.height, texture);
	Rectangle floor  = { -100, HEIGHT/2 + 40, 10000, 50 };

	std::vector<Rectangle> objects;
	std::vector<Bullet>    bullets;

	objects.push_back(floor);

	//generate map terrain
	genBuildings(objects, 100, 500, 300, 100, 100);

	Camera2D camera = {{ 0 }, {0, 0}, 0.0f, 0.0f };
	camera.zoom = 1.0f;

	SetTargetFPS(60);

	while (!WindowShouldClose()){
		game.handleKeyPresses(camera, player, objects, bullets);
		game.handlePhysics(camera, player, objects, bullets);
		game.handleDraw(objects, bullets, camera, player);
	}
	CloseWindow();

	return 0;
}
