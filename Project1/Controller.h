#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Object.h"

class Controller
{
	Object* mapped_obj{ nullptr };

public:
	bool Key[press(KEY_COUNT)];


public:
	Controller();
	~Controller();

	void MappingController(Object* obj);
	void TickEvent();


	void Debug_print();
};

