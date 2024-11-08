#include "Particle.h"



Particle::Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	SetMesh("baseParticle");
	SetMaterial("Particle");

	BeginPlayEvent();
}

Particle::~Particle()
{
}

void Particle::BeginPlayEvent()
{
	Object::BeginPlayEvent();

	setting.isStatic = false;
	setting.EnalbeGravity = false;
	setting.EnalbeCollision = false;
	setting.cast_shadow = false;
}

void Particle::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

}

void Particle::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);


}