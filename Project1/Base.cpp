#include "Base.h"

Base::Base(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer)
	: Object(obj_id, type, loc, importer)
{
	SetMesh("Base.obj");
	SetMaterial("Base");
}

Base::~Base()
{
}

void Base::BeginPlayEvent()
{
	Object::BeginPlayEvent();


}

void Base::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);


}