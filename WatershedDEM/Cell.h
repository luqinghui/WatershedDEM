#pragma once

#include <vector>

class Cell {
	static const int INIT = -1;	//��ʼ��ͼ��
	static const int MASK = -2; //ָʾ��դ��㽫������ÿ���㼶�ĳ�ʼֵ��
	static const int WSHED = 0;	//����դ�������ĳ����ˮ��
	static const int FICTITIOUS = -1;  //����դ���

	int x;
	int y;
	float elevation;  //�߳�
	int label;
	int dist;

	std::vector<Cell*> neighbours; //�洢����դ��

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

	//��ȡ�̼߳�����
	float getElevation() const { return elevation; }
	int getIntHeight() const { return (int)elevation & 0xff; }
	int getX() const { return x; }
	int getY() const { return y; }

	//���úͻ�ȡ��ǩ
	void setLabel(int label) { this->label = label; }
	void setLabelToINIT() { this->label = INIT; }
	void setLableToMASK() { this->label = MASK; }
	void setLabelToWSHED() { this->label = WSHED; }
	int getLabel() { return this->label; }

	//�жϱ�ǩ״̬
	bool isLabelINIT() { return this->label == INIT; }
	bool isLabelMASK() { return this->label == MASK; }
	bool isLabelWSHED() { return this->label == WSHED; }

	//��ȡ������λ��
	void setDistance(int dis) { this->dist = dis; }
	int getDistance() { return this->dist; }

	//�ж��Ƿ�Ϊ������Ԫ
	bool isFICTITOUS() { return this->label == FICTITIOUS; }

	bool allNeighboursAreWSHED() {
		for (unsigned i = 0; i < this->neighbours.size(); i++){
			Cell * n = neighbours.at(i);
			if (!n->isLabelWSHED()) return false;
		}
		return true;
	}
};