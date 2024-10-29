#include "Base.h"

Base::Base(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, std::vector<Object*>* _AllObjects)
	: Object(obj_id, type, loc, importer, _AllObjects)
{
	SetMesh("Base.obj");
	SetMaterial("Base");

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
	SetScale({ 50,1,50 });                                                                                                                                   
}

void Base::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	

}