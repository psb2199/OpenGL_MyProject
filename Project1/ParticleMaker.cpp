#include "ParticleMaker.h"

VertexData* ParticleMaker::CreateParticleObject(std::string name, int particle_count)
{
	VertexData* newParticle = new VertexData;


	std::vector<float> vertices_data;

	for (int i = 0; i < particle_count; ++i)
	{
		float center_x = static_cast<float>(GetRandint(-5000, 5000)) / 100.0f;
		float center_y = static_cast<float>(GetRandint(-5000, 5000)) / 100.0f;
		float center_z = static_cast<float>(GetRandint(-5000, 5000)) / 100.0f;
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

			vertices_data.push_back(start_time);

			vertices_data.push_back(center_x);
			vertices_data.push_back(center_y);
			vertices_data.push_back(center_z);
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
		pos, coord, start_time, center_pos
		// x,y,z  u,v   t   
	};
	int stride = sizeof(float) * 9;

	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(coord);
	glVertexAttribPointer(coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(start_time);
	glVertexAttribPointer(start_time, 1, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	glEnableVertexAttribArray(center_pos);
	glVertexAttribPointer(center_pos, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

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
