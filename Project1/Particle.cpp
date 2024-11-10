#include "Particle.h"

#include "ObjectManager.h"


Particle::Particle(int obj_id, int type, int p_type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	particle_type = p_type;

	SetParticleSetting();
	SetMesh(CreateParticleObject(particle_setting.particleCount));

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

	if (GetElapsedTime() > particle_setting.lifeTime)
	{
		GetWorld()->DeleteObject(this);
	}
}
void Particle::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);


}

void Particle::SetParticleSetting()
{
	switch (particle_type)
	{
	case P_PopCoin:
		SetMaterial("PM_Base");
		particle_setting.particleCount = 20;

		particle_setting.particleColor = { 1.f, 1.f, 0.5f };
		particle_setting.randomSeedValue = 0.5f;
		particle_setting.lifeTime = 0.75;
		particle_setting.fadeOutTime = 0.5;

		particle_setting.size_range =		{ 10, 30 };
		particle_setting.center_x_range =	{ -100, 100 };
		particle_setting.center_y_range =	{ -100, 100 };
		particle_setting.center_z_range =	{ -100, 100 };
		particle_setting.dir_x_range =		{ -50, 50 };
		particle_setting.dir_y_range =		{ 95, 100 };
		particle_setting.dir_z_range =		{ -50, 50 };
		particle_setting.velocity_range =	{ 1500, 3000 };
		particle_setting.randomSeed_range = { 0, 50 };
		break;

	case P_Leaf:
		setting.EnalbeTwoFace = true;
		setting.cast_shadow = true;

		SetMaterial("PM_Leaf");
		particle_setting.particleCount = 50;

		particle_setting.particleColor = { 0.34, 0.37, 0.14 };
		particle_setting.randomSeedValue = 0.5f;
		particle_setting.lifeTime = 0.5;
		particle_setting.fadeOutTime = 0.4;

		particle_setting.size_range = { 30, 40 };
		particle_setting.center_x_range = { -100, 100 };
		particle_setting.center_y_range = { -80, -80 };
		particle_setting.center_z_range = { -100, 100 };
		particle_setting.dir_x_range = { -30, 30 };
		particle_setting.dir_y_range = { 90, 100 };
		particle_setting.dir_z_range = { -30, 30 };
		particle_setting.velocity_range = { 1200, 1400 };
		particle_setting.randomSeed_range = { -100, 100 };
		break;
	}
}


void Particle::DoParticleUniform(GLuint shader)
{
	glUniform1i(glGetUniformLocation(shader, "particle_type"), particle_type);

	if (particle_setting.followObject)
	{
		glm::vec3 follow_loc = particle_setting.followObject->GetLocation();
		glUniform3f(glGetUniformLocation(shader, "follow_location"), follow_loc.x, follow_loc.y, follow_loc.z);

		glm::vec3 follow_vel = particle_setting.followObject->GetVelocity();
		glUniform3f(glGetUniformLocation(shader, "follow_velocity"), follow_vel.x, follow_vel.y, follow_vel.z);
	}
	
	glm::vec3 color = particle_setting.particleColor;
	glUniform3f(glGetUniformLocation(shader, "particleColor"),		color.r, color.g, color.b);

	glUniform1f(glGetUniformLocation(shader, "randomSeedValue"),	particle_setting.randomSeedValue);
	glUniform1f(glGetUniformLocation(shader, "lifeTime"),			particle_setting.lifeTime);
	glUniform1f(glGetUniformLocation(shader, "fadeOutTime"),		particle_setting.fadeOutTime);

}


VertexData* Particle::CreateParticleObject(int particle_count)
{
	VertexData* newParticle = new VertexData;

	std::vector<float> vertices_data;

	for (int i = 0; i < particle_count; ++i)
	{
		ParticleSetting s = particle_setting;
		float size = static_cast<float>(GetRandint(s.size_range.x, s.size_range.y)) / 100.0f;

		float center_x = static_cast<float>(GetRandint(s.center_x_range.x, s.center_x_range.y)) / 100.0f;
		float center_y = static_cast<float>(GetRandint(s.center_y_range.x, s.center_y_range.y)) / 100.0f;
		float center_z = static_cast<float>(GetRandint(s.center_z_range.x, s.center_z_range.y)) / 100.0f;

		glm::vec3 dir;
		dir.x = static_cast<float>(GetRandint(s.dir_x_range.x, s.dir_x_range.y)) / 100.0f;
		dir.y = static_cast<float>(GetRandint(s.dir_y_range.x, s.dir_y_range.y)) / 100.0f;
		dir.z = static_cast<float>(GetRandint(s.dir_z_range.x, s.dir_z_range.y)) / 100.0f;
		dir = glm::normalize(dir);

		float velocity = static_cast<float>(GetRandint(s.velocity_range.x, s.velocity_range.y)) / 100.0f;

		dir *= velocity;

		float randomSeed = static_cast<float>(GetRandint(s.randomSeed_range.x, s.randomSeed_range.y)) / 100.0f;

		for (int j = 0; j < 6; ++j)
		{
			switch (j)
			{
			case 0:
				vertices_data.push_back(-size + center_x);
				vertices_data.push_back(size + center_y);
				vertices_data.push_back(0.0f + center_z);

				vertices_data.push_back(0.0f); vertices_data.push_back(0.0f); // Texture Coord
				break;

			case 1:
				vertices_data.push_back(-size + center_x);
				vertices_data.push_back(-size + center_y);
				vertices_data.push_back(0.0f + center_z);

				vertices_data.push_back(0.0f); vertices_data.push_back(1.0f);
				break;

			case 2:
				vertices_data.push_back(size + center_x);
				vertices_data.push_back(size + center_y);
				vertices_data.push_back(0.0f + center_z);

				vertices_data.push_back(1.0f); vertices_data.push_back(0.0f);
				break;

			case 3:
				vertices_data.push_back(size + center_x);
				vertices_data.push_back(size + center_y);
				vertices_data.push_back(0.0f + center_z);

				vertices_data.push_back(1.0f); vertices_data.push_back(0.0f);
				break;

			case 4:
				vertices_data.push_back(-size + center_x);
				vertices_data.push_back(-size + center_y);
				vertices_data.push_back(0.0f + center_z);

				vertices_data.push_back(0.0f); vertices_data.push_back(1.0f);
				break;

			case 5:
				vertices_data.push_back(size + center_x);
				vertices_data.push_back(-size + center_y);
				vertices_data.push_back(0.0f + center_z);

				vertices_data.push_back(1.0f); vertices_data.push_back(1.0f);
				break;
			}

			vertices_data.push_back(randomSeed);

			vertices_data.push_back(center_x);
			vertices_data.push_back(center_y);
			vertices_data.push_back(center_z);

			vertices_data.push_back(dir.x);
			vertices_data.push_back(dir.y);
			vertices_data.push_back(dir.z);
		}

		newParticle->max_location.x = max(newParticle->max_location.x, center_x);
		newParticle->max_location.y = max(newParticle->max_location.y, center_y);
		newParticle->max_location.z = max(newParticle->max_location.z, center_z);

		newParticle->min_location.x = min(newParticle->min_location.x, center_x);
		newParticle->min_location.y = min(newParticle->min_location.y, center_y);
		newParticle->min_location.z = min(newParticle->min_location.z, center_z);
	}


	glGenVertexArrays(1, &newParticle->VAO);
	glBindVertexArray(newParticle->VAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_data.size() * sizeof(float), vertices_data.data(), GL_STATIC_DRAW);


	enum attribute {
		pos, coord, random_seed, center_pos, velocity
	};
	int stride = sizeof(float) * 12;

	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(coord);
	glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(random_seed);
	glVertexAttribPointer(random_seed, 1, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	glEnableVertexAttribArray(center_pos);
	glVertexAttribPointer(center_pos, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(velocity);
	glVertexAttribPointer(velocity, 3, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));

	glBindVertexArray(0);


	newParticle->filename = particle_type;
	newParticle->polygon_count = particle_count * 2;
	newParticle->VBO = NULL;
	newParticle->texCoordVBO = NULL;
	newParticle->normalVBO = NULL;
	newParticle->tangentVBO = NULL;
	newParticle->bitangentVBO = NULL;

	newParticle->vertexs.push_back(glm::vec3(0.0));
	newParticle->texCoords.push_back(glm::vec2(0.0));
	newParticle->normals.push_back(glm::vec3(0.0));

	newParticle->faceIndices.push_back(glm::vec3(0.0));
	newParticle->texCoordIndices.push_back(glm::vec3(0.0));
	newParticle->normalIndices.push_back(glm::vec3(0.0));

	return newParticle;
}


void Particle::SetFollowObject(Object* obj)
{
	particle_setting.followObject = obj;
}

glm::vec3 Particle::GetFollowLocation()
{
	return particle_setting.followObject->GetLocation();
}

void Particle::SetParticleType(int t)
{
	particle_type = t;
}