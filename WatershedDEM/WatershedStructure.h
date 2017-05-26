#pragma once
#include<algorithm>
#include"Cell.h"

class WatershedStructure {
	std::vector<Cell*> cells;
public:
	WatershedStructure(float* data, int width, int height) {
		cells.reserve(width*height);
		//存储栅格
		for (unsigned y = 0; y < height; y++)
			for (unsigned x = 0; x < width; x++)
				cells.push_back(new Cell(x, y, data[x + y*width]));

		//计算每个栅格邻域栅格
		for(int y=0;y<height;)

	}

};
