#pragma once
#include "OpenGL.h"

#include "Object.h"

class ObjectManager;

class Particle : public Object
{
	GLuint m_Particle_VAO;
	int m_poly_count;

	void CreateParticleObject(int particle_count);

	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;
	void OverlapedCollisionEvent(Object* collision_obj) override;
public:
	Particle(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, ObjectManager* objmgr);
	~Particle();
};

