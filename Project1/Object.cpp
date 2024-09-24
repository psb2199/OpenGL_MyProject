#include "Object.h"

Object::Object()
{
	location.x = 0;
	location.y = 0;
	location.z = 0;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
}

Object::~Object()
{
}

void Object::SetLocation(vector3 new_location)
{
	location = new_location;
}

vector3 Object::GetLocation() const
{
	return vector3();
}

void Object::SetRotation(vector3 new_rotation)
{
	rotation = new_rotation;
}

vector3 Object::GetRotation() const
{
	return vector3();
}
