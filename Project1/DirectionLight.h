#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Object.h"

class DirectionLight
{	
	std::string type;
	glm::vec3 location;
	glm::vec3 direction;
	glm::vec3 color;

	Object* attached_obj{ nullptr };

public:
	glm::vec3 GetLightColor() const;

	void SetLocation(glm::vec3 xyz);
	glm::vec3 GetLocation() const;

	void SetDirection(glm::vec3 xyz);
	glm::vec3 GetDirection() const;
	

	void LightWorks(GLuint Shader);
	void AttachDirectionLight(Object* obj);

	DirectionLight(glm::vec3 xyz);
	~DirectionLight();
};

