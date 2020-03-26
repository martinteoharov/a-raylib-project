#ifndef UTILS_H
#define UTILS_H
int totalFPS = 0;
int currFrame = 0;
int getAverageFPS(){
	currFrame ++;
	totalFPS += GetFPS();
	//std::cout << GetFPS() << std::endl;
	return totalFPS/currFrame;
}

#endif
