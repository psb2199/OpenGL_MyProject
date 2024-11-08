#pragma once
#include "OpenGL.h"

#include "Object.h"

class ObjectManager;


class Particle : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;
	void OverlapedCollisionEvent(Object* collision_obj) override;

public:
	static std::vector<VertexData*> Particles;
	Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	~Particle();
};

