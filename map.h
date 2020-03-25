#ifndef MAP_H
#define MAP_H

class Map {
	private:
	public:
		static std::vector<Rectangle> serializeRead (std::string fn);
		static void serializeWrite(std::vector<Rectangle> objects, std::string fn);
		static void listMaps(std::string path, std::vector<std::string>& stringvec);
};

std::vector<Rectangle> Map::serializeRead(std::string fn){
	std::vector<Rectangle> objects;
	std::cout << "LOADING.. " << fn << std::endl;
	std::ifstream infile(fn);
	int x, y, w, h;
	while(infile >> x >> y >> w >> h){
		Rectangle rect = {x, y, w, h};
		objects.push_back(rect);
	}
	return objects;
}

void Map::serializeWrite(std::vector<Rectangle> objects, std::string fn){
	std::cout << "SAVING.. " << fn << std::endl;
	std::ofstream outfile;
	outfile.open (fn);
	for( int i = 0; i < objects.size(); i ++ ){
		outfile << objects[i].x << " " << objects[i].y <<  " " << objects[i].width << " " << objects[i].height << std::endl;
	}

	outfile.close();
}


void Map::listMaps(std::string path, std::vector<std::string>& stringvec){
	for (const auto & entry : std::filesystem::directory_iterator(path))
		stringvec.push_back(entry.path());
}


#endif
