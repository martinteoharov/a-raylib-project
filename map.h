#ifndef MAP_H
#define MAP_H
#include "player.h"

#define GRID_SIZE 2000
#define RENDER_DISTANCE 2


class Map {
	private:
	public:
		static void createRect (Rectangle obj, std::map<int, std::vector<Rectangle>>& mObjects);
		static std::map<int, std::vector<Rectangle>> serializeRead (std::string fn, Player& player);
		static void serializeWrite(std::map<int, std::vector<Rectangle>> mObjects, Player& player, std::string fn);
		static void listMaps(std::string path, std::vector<std::string>& stringvec);
		static void drawSectors(int norm_x);
};
bool findRectInVec(Rectangle rect, std::vector<Rectangle> vec){
	for(int i = 0; i < vec.size(); i ++ ){
		if(vec[i].x == rect.x &&
			       	vec[i].width == rect.width &&
			       	vec[i].y == rect.y &&
			       	vec[i].height == rect.height) {
			return true;
		}
	}
	return false;
}




void Map::createRect (Rectangle obj, std::map<int, std::vector<Rectangle>>& mObjects){
	int normX = obj.x / GRID_SIZE;
	int normBigX = (obj.x + obj.width) / GRID_SIZE;

	while(normBigX >= normX){
		mObjects[normBigX].push_back(obj);
		normBigX --;
	}
}

std::map<int, std::vector<Rectangle>> Map::serializeRead(std::string fn, Player& player){
	std::map<int, std::vector<Rectangle>> mObjects;



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

			int normX = rect.x / GRID_SIZE;
			int normBigX = (rect.x + rect.width) / GRID_SIZE;


			while(normBigX >= normX){
				mObjects[normBigX].push_back(rect);
				normBigX --;
			}
		}
	}

	return mObjects;
}


void Map::serializeWrite(std::map<int, std::vector<Rectangle>> mObjects, Player& player, std::string fn){
	std::cout << "SAVING.. " << fn << std::endl;
	std::ofstream outfile;

	outfile.open (fn, std::ofstream::out | std::ofstream::trunc);

	//std::vector<Rectangle> objects = mObjects[player.getX() / GRID_SIZE];
	std::vector<Rectangle> objects;

	//in the future check if object is already in
	for (const auto& elem : mObjects) {
		std::vector<Rectangle> vec = elem.second;
		for(int i = 0; i < vec.size(); i ++ ){
			if(!findRectInVec(vec[i], objects))
				objects.push_back(vec[i]);
		}
	}


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
void Map::drawSectors(int norm_x){
	for(int i = 0; i < 100; i ++ ){
		Rectangle line;
		line.x = i * GRID_SIZE;
		line.y = -1000;
		line.width = 5;
		line.height = 2000;

		if(norm_x == i || norm_x == i - 1){
			//active
			DrawRectangleRec(line, DARKGREEN);
		}else{
			DrawRectangleRec(line, GREEN);
		}

	}
}



#endif
