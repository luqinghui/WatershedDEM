#pragma once
#include<algorithm>
#include"Cell.h"

class WatershedStructure {
	std::vector<Cell*> cells;
public:
	WatershedStructure(float* data, int width, int height) {
		cells.reserve(width*height);
		//´æ´¢Õ¤¸ñ
		for (unsigned y = 0; y < height; y++)
			for (unsigned x = 0; x < width; x++)
				cells.push_back(new Cell(x, y, data[x + y*width]));

		std::sort(cells.begin(), cells.end(), [](Cell *pl, Cell *pr) {return pl < pr; });
	}

	~WatershedStructure() {
		while (!cells.empty()) {
			Cell *p = cells.back();
			delete p;
			p = NULL;
			cells.pop_back();
		}
	}

	int size() { return cells.size(); }
	Cell * at(int i) { return cells.at(i); }
};
