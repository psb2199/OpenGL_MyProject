#include "Controller.h"

Controller::Controller()
{
	for (int i{ 0 }; i < press(KEY_COUNT); ++i) { Key[i] = false; }
}

Controller::~Controller()
{
}

void Controller::MappingController(Object* obj)
{
	mapped_obj = obj;
}

void Controller::TickEvent()
{
	if (Key[press(w)])
	{
		mapped_obj->AddMovementInput({ 0,0,0.1 });
	}
	if (Key[press(s)])
	{
		mapped_obj->AddMovementInput({ 0,0,-0.1 });
	}
	if (Key[press(a)])
	{
		mapped_obj->AddMovementInput({ -0.1,0,0 });
	}
	if (Key[press(d)])
	{
		mapped_obj->AddMovementInput({ 0.1,0,0 });
	}

	//mapped_obj->GetLocation().print_vec3();
}

void Controller::Debug_print()
{
	for (int i{ 0 }; i < press(KEY_COUNT); ++i)
	{
		std::cout << Key[i];
	}
	std::cout << std::endl;
}


