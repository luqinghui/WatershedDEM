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

		for (int y = 0; y < height; ++y) {
		
			for (int x = 0; x < width; ++x) {
				int current_index = x + y*width;	// µ±Ç°ÏñËØµãË÷Òý
				Cell* current_cell = cells.at(current_index);

				if (x - 1 >= 0) {
					current_cell->addNeighbour(cells.at(current_index - 1)); // ×óÁÚÓò
					if (y - 1 >= 0)	 // ×óÏÂ½Ç
						current_cell->addNeighbour(cells.at(current_index - 1 - width));
					if (y + 1 < height)	 // ×óÉÏ½Ç
						current_cell->addNeighbour(cells.at(current_index - 1 + width));
				}

				if (x + 1 < width) {
					current_cell->addNeighbour(cells.at(current_index + 1)); // ÓÒÁÚÓò
					if (y - 1 >= 0)	 // ÓÒÏÂ½Ç
						current_cell->addNeighbour(cells.at(current_index + 1 - width));
					if (y + 1 < height) // ÓÒÉÏ½Ç
						current_cell->addNeighbour(cells.at(current_index + 1 + width));
				}

				if (y - 1 >= 0) // ÏÂÁÚÓò
					current_cell->addNeighbour(cells.at(current_index - width));

				if (y + 1 < height)	 // ÉÏÁÚÓò
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
