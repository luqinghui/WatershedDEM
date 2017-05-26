#pragma once

#include <vector>

class Cell {
	static const int INIT = -1;	//初始化图像
	static const int MASK = -2; //指示新栅格点将被处理（每个层级的初始值）
	static const int WSHED = 0;	//表明栅格点属于某个分水岭
	static const int FICTITIOUS = -1;  //虚拟栅格点

	int x;
	int y;
	float elevation;  //高程
	int label;
	int dist;

	std::vector<Cell*> neighbours; //存储邻域栅格

public:
	Cell(int x, int y, float elevation) {
		this->x = x;
		this->y = y;
		this->elevation = elevation;
		this->label = INIT;
		this->dist = 0;
		neighbours.reserve(8);
	}
	Cell() { label = FICTITIOUS; }
	void addNeighbour(Cell *neighbour) { neighbours.push_back(neighbour); }
	std::vector<Cell *>& getNeighbours() { return neighbours; }

	//获取高程及坐标
	float getElevation() const { return elevation; }
	int getIntHeight() const { return (int)elevation & 0xff; }
	int getX() const { return x; }
	int getY() const { return y; }

	//设置和获取标签
	void setLabel(int label) { this->label = label; }
	void setLabelToINIT() { this->label = INIT; }
	void setLableToMASK() { this->label = MASK; }
	void setLabelToWSHED() { this->label = WSHED; }
	int getLabel() { return this->label; }

	//判断标签状态
	bool isLabelINIT() { return this->label == INIT; }
	bool isLabelMASK() { return this->label == MASK; }
	bool isLabelWSHED() { return this->label == WSHED; }

	//获取和设置位置
	void setDistance(int dis) { this->dist = dis; }
	int getDistance() { return this->dist; }

	//判断是否为虚拟像元
	bool isFICTITOUS() { return this->label == FICTITIOUS; }

	bool allNeighboursAreWSHED() {
		for (unsigned i = 0; i < this->neighbours.size(); i++){
			Cell * n = neighbours.at(i);
			if (!n->isLabelWSHED()) return false;
		}
		return true;
	}
};