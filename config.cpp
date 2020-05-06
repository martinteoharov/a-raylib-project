#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "config.h"

namespace config {
	int FPS = 60;
	int PLAYER_WIDTH = 50;
	int PLAYER_HEIGHT = 40;
	int SCREEN_WIDTH = 1366;
	int SCREEN_HEIGHT = 768;
	std::string TEXT_STYLE = "CANDY.RGS";
	int SHADER = 0;
}


bool config::LOAD_CONFIG(std::string fn){
        std::cout << "LOADING CONFIGURATION FILE.." << fn << std::endl;
        std::ifstream infile(fn);

	std::map<int, std::string> mTextStyle;
	mTextStyle[0] = "CHERRY.RGS";
	mTextStyle[1] = "CANDY.RGS";

        std::string name;
        int val;

        while(infile >> name >> val){
                if(name == "FPS")
                        config::FPS = val;
                else if(name == "PLAYER_WIDTH")
                        config::PLAYER_WIDTH = val;
                else if(name == "PLAYER_HEIGHT")
                        config::PLAYER_HEIGHT = val;
                else if(name == "SCREEN_WIDTH")
                        config::SCREEN_WIDTH = val;
                else if(name == "SCREEN_HEIGHT")
                        config::SCREEN_HEIGHT = val;
                else if(name == "TEXT_STYLE")
                        config::TEXT_STYLE = mTextStyle[val];
                else if(name == "SHADER")
                        config::SHADER = val;
        }
	return true;
}       
