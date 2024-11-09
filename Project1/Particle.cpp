#include "Particle.h"

#include "ObjectManager.h"


Particle::Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	SetMesh(CreateParticleObject("baseParticle", 50));
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



	if (GetElapsedTime() > lifeTime)
	{
		GetWorld()->DeleteObject(this);
	}
}
void Particle::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);


}

void Particle::DoParticleUniform(GLuint shader)
{
	glm::vec3 follow_loc = followObject->GetLocation();
	glUniform3f(glGetUniformLocation(shader, "follow_location"), follow_loc.x, follow_loc.y, follow_loc.z);

	glUniform3f(glGetUniformLocation(shader, "particleColor"), particleColor.r, particleColor.g, particleColor.b);

	glUniform1f(glGetUniformLocation(shader, "randomSeedValue"), randomSeedValue);
	glUniform1f(glGetUniformLocation(shader, "lifeTime"), lifeTime);
	glUniform1f(glGetUniformLocation(shader, "fadeOutTime"), fadeOutTime);
}


VertexData* Particle::CreateParticleObject(std::string name, int particle_count)
{
	particleColor = { 1.f, 1.f, 0.5f };
	randomSeedValue = 0.5f;
	lifeTime = 0.75;
	fadeOutTime = 0.5;

	VertexData* newParticle = new VertexData;

	std::vector<float> vertices_data;

	for (int i = 0; i < particle_count; ++i)
	{
		//static_cast<float>(GetRandint(-5000, 5000)) / 100.0f;

		float size = static_cast<float>(GetRandint(10, 30)) / 100.0f;

		float center_x = static_cast<float>(GetRandint(-100, 100)) / 100.0f;
		float center_y = static_cast<float>(GetRandint(-100, 100)) / 100.0f;
		float center_z = static_cast<float>(GetRandint(-100, 100)) / 100.0f;

		glm::vec3 dir;
		dir.x = static_cast<float>(GetRandint(-50, 50)) / 100.0f;
		dir.y = static_cast<float>(GetRandint(95, 100)) / 100.0f;
		dir.z = static_cast<float>(GetRandint(-50, 50)) / 100.0f;
		dir = glm::normalize(dir);

		float velocity = static_cast<float>(GetRandint(1500, 3000)) / 100.0f;

		dir *= velocity;

		float randomSeed = static_cast<float>(GetRandint(0, 50)) / 100.0f;

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
		// x,y,z  u,v   t   
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


	newParticle->filename = name;
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
	followObject = obj;
}

glm::vec3 Particle::GetFollowLocation()
{
	return followObject->GetLocation();
}