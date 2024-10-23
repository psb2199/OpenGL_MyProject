#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Object.h"


class Controller
{
	Object* mapped_obj{ nullptr };

	glm::vec3 controller_direction;


public:
	bool Key[press(KEY_COUNT)];


public:
	Controller();
	~Controller();

	void MappingController(Object* obj);
	void TickEvent(float delta_seconds);


	void Debug_print();
};

