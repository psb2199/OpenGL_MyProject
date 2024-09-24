#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

Object* ObjectManager::AddObject(std::string type, vector3 location)
{
    Object* newObj = new Object(AllOjectCount, type, location);

	WorldObjects.push_back(newObj);
    AllOjectCount++;

    return newObj;
}

void ObjectManager::DeleteObject(Object* obj)
{
    auto it = std::find(WorldObjects.begin(), WorldObjects.end(), obj);

    if (it != WorldObjects.end()) {
        WorldObjects.erase(it);
        AllOjectCount--;
    }

}

std::vector<Object*> ObjectManager::GetAllObjects()
{
	return WorldObjects;
}
