#pragma once
#include <iostream>

#include "OpenGL.h"

class Light
{	
	std::string type;
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 color;


public:
	glm::vec3 GetLocation() const;
	glm::vec3 GetLightColor() const;

	Light(glm::vec3 xyz);
	~Light();
};

