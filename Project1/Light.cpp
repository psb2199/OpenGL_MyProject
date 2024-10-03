#include "Light.h"


Light::Light(glm::vec3 xyz)
{
	type = "none";
	location = xyz;
	rotation = glm::vec3{ 0 };
	color = glm::vec3{ 1.f, 1.f, 1.f };
	distance = 100.f;
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

float Light::GetLightDistance() const
{
	return distance;
}
