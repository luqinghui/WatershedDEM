#include<iostream>

#include"WatershedAlgorithm.h"


int main(void)
{
	std::cout << "Watershed:" << std::endl;
	
	std::string input = "D:\\TempData\\Tibet.tif";
	std::string output = "D:\\TempData\\Tibet_ws_label.tif";

	WatershedAlgorithm wa(input, output);
	wa.run();

	return 0;

}