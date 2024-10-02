#include "Light.h"


Light::Light(glm::vec3 xyz)
{
	type = "none";
	location = xyz;
	rotation = glm::vec3{ 0 };
	color = glm::vec3{ 1.f, 0.f, 0.f };
	distance = 0.5;
}

Light::~Light()
{
}

glm::vec3 Light::GetLocation() const
{
	return location;
}

glm::vec3 Light::GetLightColor() const
{
	return color;
}
