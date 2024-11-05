
#include "Base.h"

Base::Base(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	SetMesh("Base.obj");
	SetMaterial("Grass");

	BeginPlayEvent();
}

Base::~Base()
{
}

void Base::BeginPlayEvent()
{
	Object::BeginPlayEvent();

	setting.isStatic = true;
	setting.EnalbeCollision = true;
	setting.EnableRendering = true;
	SetScale({ 10,1,10 });                                                                                                                                   
}

void Base::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	

}