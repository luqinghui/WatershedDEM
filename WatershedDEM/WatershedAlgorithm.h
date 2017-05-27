#pragma once
#include<iostream>
#include<string>
#include<queue>
#include"WatershedStructure.h"

#include<gdal_priv.h>
#include<cpl_conv.h>

class WatershedAlgorithm{
	float HMIN;
	float HMAX;
	int width;
	int height;
	float *data;

public:
	WatershedAlgorithm(const std::string& inImg, const std::string& outImg) {
		GDALAllRegister();
		GDALDataset *poDataset = (GDALDataset*)GDALOpen(inImg.c_str(), GA_ReadOnly);
		if (poDataset == NULL) {
			std::cout << "���ļ�ʧ��" << std::endl;
			return;
		}
		width = poDataset->GetRasterXSize();
		height = poDataset->GetRasterYSize();
		GDALRasterBand *poBand = poDataset->GetRasterBand(1);

		data = new float[width*height];
		poBand->RasterIO(GF_Read, 0, 0, width, height, data, width, height, GDT_Float32, 0, 0);

		HMIN = poBand->GetMinimum();
		HMAX = poBand->GetMaximum();
	}

	void run() {
		//��һ��:��դ�����ṹ�岢����
		WatershedStructure watershedStructure(data, width, height);

		//�ڶ���:ģ���û
		std::queue<Cell*> cell_queue;	//�洢դ�����ʱ����
		int current_label = 0;
		int height_index1 = 0;
		int height_index2 = 0;
		
		for (int h = HMIN; h < HMAX; ++h) {	//h-1���Geodesic SKIZ
			for (int cell_index = height_index1; cell_index < watershedStructure.size(); ++cell_index) {
				Cell* c = watershedStructure.at(cell_index);

				if (c->getElevation != h) { height_index1 = cell_index; break; }	//��դ��λ��h+1�㣬�ݲ���������ѭ��

				c->setLableToMASK();

				//����դ��
				std::vector<Cell*> neighbours;


			}

		}

	}
};