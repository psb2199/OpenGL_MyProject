#pragma once
#include "OpenGL.h"

#include "Object.h"

#include "Particle.h"

#include "Player.h"
#include "Coin.h"
#include "Base.h"

class ObjectManager
{
	std::vector<Object*> WorldObjects;
	int AllOjectCount{ 0 };
	Importer* m_importer;

public:
	ObjectManager(Importer* importer);
	~ObjectManager();

	Object* AddObject(int type, glm::vec3 location);
	Object* SpawnParticle(int type, glm::vec3 location);
	void DeleteObject(Object* obj);
	std::vector<Object*> GetAllObjects();
};

