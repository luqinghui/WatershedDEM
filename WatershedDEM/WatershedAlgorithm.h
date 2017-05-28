#pragma once
#include<iostream>
#include<string>
#include<queue>
#include"WatershedStructure.h"

#include<gdal_priv.h>
#include<cpl_conv.h>

class WatershedAlgorithm {
	float HMIN;
	float HMAX;
	int width;
	int height;
	float *data;

	GDALDataset *inputDataset;
	GDALDataset *outputDataset;

	std::string inImg;
	std::string outImg;

public:
	WatershedAlgorithm(const std::string& in_image, const std::string& out_image) {
		this->inImg = in_image;
		this->outImg = out_image;
		GDALAllRegister();
		inputDataset = (GDALDataset*)GDALOpen(inImg.c_str(), GA_ReadOnly);
		if (inputDataset == NULL) {
			std::cout << "打开文件失败" << std::endl;
			return;
		}
		width = inputDataset->GetRasterXSize();
		height = inputDataset->GetRasterYSize();
		GDALRasterBand *poBand = inputDataset->GetRasterBand(1);

		data = new float[width*height];
		poBand->RasterIO(GF_Read, 0, 0, width, height, data, width, height, GDT_Float32, 0, 0);

		HMIN = poBand->GetMinimum();
		HMAX = poBand->GetMaximum();
	}

	void run() {
		//第一步:将栅格存入结构体并排序
		WatershedStructure watershedStructure(data, width, height);

		//第二步:模拟浸没
		std::queue<Cell*> cell_queue;	//存储栅格的临时队列
		int current_label = 0;
		int height_index1 = 0;
		int height_index2 = 0;

		for (int h = HMIN; h < HMAX; ++h) {	//h-1层的Geodesic SKIZ
			for (int cell_index = height_index1; cell_index < watershedStructure.size(); ++cell_index) {
				Cell* c = watershedStructure.at(cell_index);

				if (c->getElevation() != h) { height_index1 = cell_index; break; }	//此栅格位于h+1层，暂不处理，跳出循环

				c->setLableToMASK();

				//邻域栅格
				std::vector<Cell*> neighbours = c->getNeighbours();
				for (unsigned i = 0; i < neighbours.size(); i++) {
					Cell* q = neighbours.at(i);
					if (q->getLabel() >= 0) {
						c->setDistance(1);
						cell_queue.push(c);
						break;
					}
				}
			}

			int current_distance = 1;

			cell_queue.push(new Cell());

			while (true) {
				Cell *p = cell_queue.front();
				cell_queue.pop();

				if (p->isFICTITOUS()) {
					if (cell_queue.empty()) {
						delete p;
						p = NULL;
						break;
					}
					else {
						cell_queue.push(new Cell());
						current_distance++;
						delete p;
						p = cell_queue.front();
						cell_queue.pop();
					}
				}

				std::vector<Cell*> neighbours = p->getNeighbours();
				for (unsigned i = 0; i < neighbours.size(); ++i) {
					Cell* q = neighbours.at(i);

					/* q属于一个存在的盆地或分水线 */
					if ((q->getDistance() <= current_distance) && (q->getLabel() >= 0)) {

						if (q->getLabel() > 0) {
							if (p->isLabelMASK())
								p->setLabel(q->getLabel());
							else if (p->getLabel() != q->getLabel())
								p->setLabelToWSHED();
						}
						else if (p->isLabelMASK())
							p->setLabelToWSHED();
					}
					else if (q->isLabelMASK() && (q->getDistance() == 0)) {
						q->setDistance(current_distance + 1);
						cell_queue.push(q);
					}
				}
			}

			/* 搜寻并处理h层中新的最小值 */
			for (int pixelIndex = height_index2; pixelIndex < watershedStructure.size(); pixelIndex++) {
				Cell* p = watershedStructure.at(pixelIndex);

				/* 此像素点位于 h+1 层，暂不处理，跳出循环 */
				if (p->getIntHeight() != h) { height_index2 = pixelIndex; break; }

				p->setDistance(0); // 重置距离为0

				if (p->isLabelMASK()) { // 该像素位于新最小值区域
					current_label++;
					p->setLabel(current_label);
					cell_queue.push(p);

					while (!cell_queue.empty()) {
						Cell* q = cell_queue.front();
						cell_queue.pop();

						std::vector<Cell*> neighbours = q->getNeighbours();

						for (unsigned i = 0; i < neighbours.size(); i++) { // 检查p2的邻域像素
							Cell* r = neighbours.at(i);

							if (r->isLabelMASK()) { r->setLabel(current_label); cell_queue.push(r); }
						}
					}
				}
			}
		}

		//保存结果
		GDALDriver *poDriver;
		const char *pszFormat = "GTiff";
		poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

		if (poDriver == NULL) {
			std::cout << "创建文件失败" << std::endl;
			return;
		}

		char **papszOptions = NULL;
		outputDataset = poDriver->CreateCopy(outImg.c_str(), inputDataset, false, papszOptions, NULL, NULL);

		for (int i = 0; i < watershedStructure.size(); i++) {
			Cell* p = watershedStructure.at(i);
			if (p->isLabelWSHED() && !p->allNeighboursAreWSHED())
				data[p->getX() + p->getY()*width] = 1;
			else
				data[p->getX() + p->getY()*width] = 0;

		}

		GDALRasterBand *outputBand = outputDataset->GetRasterBand(1);
		outputBand->RasterIO(GF_Write, 0, 0, width, height, data, width, width, GDT_Float32, 0, 0);
		GDALClose(inputDataset);
		GDALClose(outputDataset);
	}
};