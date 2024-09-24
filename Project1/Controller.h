#pragma once
#include <iostream>

#include "OpenGL.h"

class Controller
{

public:
	bool Key[press(KEY_COUNT)];


public:
	Controller();
	~Controller();

	void MappingController();

	void Debug_print();
};

