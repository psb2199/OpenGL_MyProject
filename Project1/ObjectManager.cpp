#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::AddObject(Object obj)
{
	WorldObjects.push_back(obj);
}

void ObjectManager::DeleteObject(Object obj)
{
    auto it = std::find(WorldObjects.begin(), WorldObjects.end(), obj);

    if (it != WorldObjects.end()) {
        WorldObjects.erase(it);
    }
}

std::vector<Object> &ObjectManager::GetAllObjects()
{
	return WorldObjects;
}
