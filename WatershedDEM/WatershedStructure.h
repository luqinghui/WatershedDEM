#pragma once
#include<algorithm>
#include"Cell.h"

class WatershedStructure {
	std::vector<Cell*> cells;
public:
	WatershedStructure(float* data, int width, int height) {
		cells.reserve(width*height);
		//�洢դ��
		for (unsigned y = 0; y < height; y++)
			for (unsigned x = 0; x < width; x++)
				cells.push_back(new Cell(x, y, data[x + y*width]));

		//����ÿ��դ������դ��
		for(int y=0;y<height;)

	}

};
