#pragma once
#include "OpenGL.h"

#include "Object.h"



class Particle : public Object
{
	void						BeginPlayEvent() override;
	void						TickEvent(float delta_sceconds) override;
	void						OverlapedCollisionEvent(Object* collision_obj) override;

	VertexData*					CreateParticleObject(std::string name, int particle_count);

	glm::vec3					particleColor{ glm::vec3(0.f)};
	float						randomSeedValue;
	float						lifeTime;
	float						fadeOutTime;

	Object*						followObject;

public:
	void						DoParticleUniform(GLuint shader);

	void						SetFollowObject(Object* obj);
	glm::vec3					GetFollowLocation();



	Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	~Particle();
};

