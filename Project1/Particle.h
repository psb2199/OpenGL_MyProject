#pragma once
#include "OpenGL.h"

#include "Object.h"

class ObjectManager;


class Particle : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;
	void OverlapedCollisionEvent(Object* collision_obj) override;

	glm::vec3 followLocation;
public:
	void SetFollowObject(Object* obj);
	glm::vec3 GetFollowLocation();

	Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	~Particle();
};

