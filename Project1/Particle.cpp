#include "Particle.h"

#include "ObjectManager.h"

void Particle::SetFollowObject(Object* obj)
{
	followLocation = obj->GetLocation();
}

glm::vec3 Particle::GetFollowLocation()
{
	return followLocation;
}

Particle::Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	SetMesh(ParticleMaker::CreateParticleObject("baseParticle", 1));
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
	setting.EnalbeCollision = true;
	setting.cast_shadow = false;
}

void Particle::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);


	if (GetElapsedTime() > 1.5)
	{
		GetWorld()->DeleteObject(this);
	}
}

void Particle::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);


}