#include "Coin.h"

Coin::Coin(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	SetMesh("Coin.obj");
	SetMaterial("Coin");

	BeginPlayEvent();
}

Coin::~Coin()
{
}

void Coin::BeginPlayEvent()
{
	Object::BeginPlayEvent();

	setting.isStatic = false;
	setting.EnalbeCollision = true;
	setting.EnableRendering = true;

	SetScale(glm::vec3(2.0));
}

void Coin::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	AddRotationInput({ 0,1,0 });

}
