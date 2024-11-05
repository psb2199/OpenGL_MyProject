#include "Particle.h"

void Particle::CreateParticleObject(int particle_count)
{
	m_poly_count = particle_count * 2;

	std::vector<float> vertices_data;

	for (int i = 0; i < particle_count; ++i)
	{
		float center_x = static_cast<float>(GetRandint(0, 100)) / 100.0f;
		float center_y = static_cast<float>(GetRandint(0, 100)) / 100.0f;
		float center_z = static_cast<float>(GetRandint(0, 100)) / 100.0f;
		float size = 0.1;
		float start_time = static_cast<float>(GetRandint(0, 100)) / 100.0f;

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

			vertices_data.push_back(start_time); // Start time
		}
	}

	enum attribute {
		pos, coord, start_time
	};

	glGenVertexArrays(1, &m_Particle_VAO);
	glBindVertexArray(m_Particle_VAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_data.size() * sizeof(float), vertices_data.data(), GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (6), (void*)0);

	// Texture coordinate attribute
	glEnableVertexAttribArray(coord);
	glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * (6), (void*)(3 * sizeof(float)));

	// Start time attribute
	glEnableVertexAttribArray(start_time);
	glVertexAttribPointer(start_time, 1, GL_FLOAT, GL_FALSE, sizeof(float) * (6), (void*)(5 * sizeof(float)));

	glBindVertexArray(0);
}


Particle::Particle(int obj_id, std::string type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	CreateParticleObject(100);

	SetMeshForParticle(m_Particle_VAO, m_poly_count);
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