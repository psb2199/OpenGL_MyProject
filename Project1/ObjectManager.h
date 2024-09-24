#pragma once
#include <iostream>
#include <vector>
#include <algorithm> 

#include "Object.h"

class ObjectManager
{
	std::vector<Object> WorldObjects;

public:
	ObjectManager();
	~ObjectManager();

	void AddObject(Object obj);
	void DeleteObject(Object obj);
	std::vector<Object> &GetAllObjects();
};

