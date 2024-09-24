#include "Controller.h"

Controller::Controller()
{
	for (int i{ 0 }; i < press(KEY_COUNT); ++i) { Key[i] = false; }
}

Controller::~Controller()
{
}

void Controller::MappingController()
{
	


}

void Controller::Debug_print()
{
	for (int i{ 0 }; i < press(KEY_COUNT); ++i)
	{
		std::cout << Key[i];
	}
	std::cout << std::endl;
}


