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

    if      (type == "Player")
    {
        newObj = dynamic_cast<Object*>(new Player(AllOjectCount, type, location, m_importer));
    }
    else if (type == "Base")
    {
        newObj = dynamic_cast<Object*>(new Base(AllOjectCount, type, location, m_importer));
    }
    else
    {
        cout << "There is no c++ class about " << type << endl;
        return nullptr;
    }
 

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
