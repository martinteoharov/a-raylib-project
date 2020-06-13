#include "sound.h"
#include "raylib.h"

namespace sound {
	Sound def_shoot_fast = LoadSound("resources/sounds/shoot.wav");
	Sound def_explode_fast = LoadSound("resources/sounds/explode.wav");
}


bool sound::LOAD_SOUND(){
	sound::def_shoot_fast = LoadSound("resources/sounds/shoot.wav");
	sound::def_explode_fast = LoadSound("resources/sounds/explode.wav");

	return true;
}       
