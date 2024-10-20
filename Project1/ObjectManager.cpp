#include "ObjectManager.h"

ObjectManager::ObjectManager(Importer_obj* importer)
{
    m_importer = importer;
}

ObjectManager::~ObjectManager()
{
}

Object* ObjectManager::AddObject(std::string type, glm::vec3 location)
{
    Object* newObj;

    
    newObj = new Object(AllOjectCount, type, location, m_importer);
 

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
