#pragma once
#include <iostream>
#include <vector>
#include <algorithm> 

#include "Object.h"

class ObjectManager
{
	std::vector<Object*> WorldObjects;
	int AllOjectCount{ 0 };

public:
	ObjectManager();
	~ObjectManager();

	Object* AddObject(std::string type, vector3 location);
	void DeleteObject(Object* obj);
	std::vector<Object*> GetAllObjects();
};

