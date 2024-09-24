#pragma once
#include <iostream>

#include "OpenGL.h"

class Object
{
	vector3 location;
	vector3 rotation;

public:
	Object();
	~Object();

	void SetLocation(vector3 new_location);
	vector3 GetLocation() const;
	void SetRotation(vector3 new_rotation);
	vector3 GetRotation() const;
	
};

