#include<iostream>

#include"WatershedAlgorithm.h"


int main(void)
{
	std::cout << "Watershed:" << std::endl;
	
	std::string input = "D:\\TempData\\watershed\\gel.tif";
	std::string output = "D:\\TempData\\watershed\\gel_out.tif";

	WatershedAlgorithm wa(input, output);
	wa.run();

	return 0;

}