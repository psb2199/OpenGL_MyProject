#include "ObjectManager.h"

ObjectManager::ObjectManager(Importer* importer)
{
    m_importer = importer;
}

ObjectManager::~ObjectManager()
{
}

Object* ObjectManager::AddObject(int type, glm::vec3 location)
{
    Object* newObj;

    switch (type)
    {
    case type_Player:
        newObj = dynamic_cast<Object*>(new Player(AllOjectCount, type, location, m_importer, this));
        break;

    case type_Base:
        newObj = dynamic_cast<Object*>(new Base(AllOjectCount, type, location, m_importer, this));
        break;

    case type_Coin:
        newObj = dynamic_cast<Object*>(new Coin(AllOjectCount, type, location, m_importer, this));
        break;
    }


	WorldObjects.push_back(newObj);
    AllOjectCount++;

    return newObj;
}

Object* ObjectManager::SpawnParticle(int type, glm::vec3 location)
{   
    Particle* newParticle = new Particle(AllOjectCount, type_Particle, type, location, m_importer, this);

    Object* newObj;

    newObj = dynamic_cast<Object*>(newParticle);

    WorldObjects.push_back(newObj);
    AllOjectCount++;

    return newObj;
}

void ObjectManager::DeleteObject(Object* obj)
{
    auto it = std::find(WorldObjects.begin(), WorldObjects.end(), obj);

    if (it != WorldObjects.end()) {
        delete (*it);
        WorldObjects.erase(it);
        AllOjectCount--;
    }

}

std::vector<Object*> ObjectManager::GetAllObjects()
{
	return WorldObjects;
}
