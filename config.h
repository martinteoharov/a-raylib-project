#ifndef CONFIG_H
#define CONFIG_H

namespace config {
	extern int FPS;
	extern int PLAYER_WIDTH;
	extern int PLAYER_HEIGHT;
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;
	extern int SHADER;

	bool LOAD_CONFIG(std::string fn);
}

#endif
