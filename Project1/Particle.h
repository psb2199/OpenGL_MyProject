#pragma once
#include "OpenGL.h"

#include "Object.h"

struct ParticleSetting
{
	int							particleCount		{ 1 };
	glm::vec3					particleColor		{ glm::vec3(0.f) };
	float						randomSeedValue		{ 0.0 };
	float						lifeTime			{ 1.0 };
	float						fadeOutTime			{ 0.0 };

	glm::vec2					size_range			{ glm::vec2(100) };
	glm::vec2					center_x_range		{ glm::vec2(0) };
	glm::vec2					center_y_range		{ glm::vec2(0) };
	glm::vec2					center_z_range		{ glm::vec2(0) };
	glm::vec2					dir_x_range			{ glm::vec2(0) };
	glm::vec2					dir_y_range			{ glm::vec2(0) };
	glm::vec2					dir_z_range			{ glm::vec2(0) };
	glm::vec2					velocity_range		{ glm::vec2(0) };
	glm::vec2					randomSeed_range	{ glm::vec2(0) };

	Object*						followObject		{ nullptr };
};

class Particle : public Object
{
	void						BeginPlayEvent() override;
	void						TickEvent(float delta_sceconds) override;
	void						OverlapedCollisionEvent(Object* collision_obj) override;

	VertexData*					CreateParticleObject(int particle_count);

	int							particle_type;
	ParticleSetting				particle_setting;
	void						SetParticleSetting();

public:
	void						SetParticleType(int t);

	void						DoParticleUniform(GLuint shader);

	void						SetFollowObject(Object* obj);
	glm::vec3					GetFollowLocation();



	Particle(int obj_id, int type, int p_type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	~Particle();
};

