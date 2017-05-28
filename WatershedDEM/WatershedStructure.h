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

		for (int y = 0; y < height; ++y) {
		
			for (int x = 0; x < width; ++x) {
				int current_index = x + y*width;	// ��ǰ���ص�����
				Cell* current_cell = cells.at(current_index);

				if (x - 1 >= 0) {
					current_cell->addNeighbour(cells.at(current_index - 1)); // ������
					if (y - 1 >= 0)	 // ���½�
						current_cell->addNeighbour(cells.at(current_index - 1 - width));
					if (y + 1 < height)	 // ���Ͻ�
						current_cell->addNeighbour(cells.at(current_index - 1 + width));
				}

				if (x + 1 < width) {
					current_cell->addNeighbour(cells.at(current_index + 1)); // ������
					if (y - 1 >= 0)	 // ���½�
						current_cell->addNeighbour(cells.at(current_index + 1 - width));
					if (y + 1 < height) // ���Ͻ�
						current_cell->addNeighbour(cells.at(current_index + 1 + width));
				}

				if (y - 1 >= 0) // ������
					current_cell->addNeighbour(cells.at(current_index - width));

				if (y + 1 < height)	 // ������
					current_cell->addNeighbour(cells.at(current_index + width));
			}
		}

		std::sort(cells.begin(), cells.end(), [](Cell *pl, Cell *pr) {return pl->getElevation() < pr->getElevation(); });
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
