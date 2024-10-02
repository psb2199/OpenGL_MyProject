#include "Light.h"

glm::vec3 Light::GetLocation() const
{
	return location;
}

glm::vec3 Light::GetLightColor() const
{
	return color;
}

Light::Light(glm::vec3 xyz)
{
	type = "none";
	location = xyz;
	rotation = glm::vec3{ 0 };
}

Light::~Light()
{
}
