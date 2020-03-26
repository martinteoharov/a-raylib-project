#ifndef MAP_H
#define MAP_H
#include "player.h"


class Map {
	private:
	public:
		static std::vector<Rectangle> serializeRead (std::string fn, Player& player);
		static void serializeWrite(std::vector<Rectangle> objects, Player& player, std::string fn);
		static void listMaps(std::string path, std::vector<std::string>& stringvec);
};

std::vector<Rectangle> Map::serializeRead(std::string fn, Player& player){
	std::vector<Rectangle> objects;
	std::cout << "LOADING.. " << fn << std::endl;
	std::ifstream infile(fn);

	std::string type;
	int x, y, w, h;
	while(infile >> type >> x >> y >> w >> h){
		if(type == "PLAYER"){
			player.setX(x);
			player.setY(y);
		}
		else if(type == "OBJECT"){
			Rectangle rect = {x, y, w, h};
			objects.push_back(rect);
		}
	}
	return objects;
}

void Map::serializeWrite(std::vector<Rectangle> objects, Player& player, std::string fn){
	std::cout << "SAVING.. " << fn << std::endl;
	std::ofstream outfile;
	outfile.open (fn);
	outfile << "PLAYER" << " " << player.getX() << " " << player.getY() <<  " " << 0 <<  " " << 0 << std::endl;
	for( int i = 0; i < objects.size(); i ++ ){
		outfile << "OBJECT" << " " <<  objects[i].x << " " << objects[i].y <<  " " << objects[i].width << " " << objects[i].height << std::endl;
	}

	outfile.close();
}


void Map::listMaps(std::string path, std::vector<std::string>& stringvec){
	for (const auto & entry : std::filesystem::directory_iterator(path)){
		std::string temp = entry.path();
		if(temp.find(".map") != std::string::npos){
			stringvec.push_back(entry.path());
		}
	}
}


#endif
