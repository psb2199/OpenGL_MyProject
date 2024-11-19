
#include "Base.h"

Base::Base(int obj_id, int type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	SetMesh("Block.obj");
	SetMaterial("Block");

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
}

void Base::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	

}