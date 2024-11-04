#include "Particle.h"

void Particle::CreateParticleObject(int particle_count)
{
	std::vector<float> verteices_data;

	for (int i{ 0 }; i < particle_count; ++i)
	{
		float size = 0.25;
		float start_time = (float)GetRandint(0, 100) / 100.0;

		for (int j{ 0 }; j < 6; ++j)
		{
			switch (j)
			{
			case 0:
				verteices_data.push_back(-size);
				verteices_data.push_back(size);
				verteices_data.push_back(0.0);

				verteices_data.push_back(0.0);
				verteices_data.push_back(0.0);
				break;

			case 1:
				verteices_data.push_back(-size);
				verteices_data.push_back(-size);
				verteices_data.push_back(0.0);

				verteices_data.push_back(0.0);
				verteices_data.push_back(1.0);
				break;

			case 2:
				verteices_data.push_back(size);
				verteices_data.push_back(size);
				verteices_data.push_back(0.0);

				verteices_data.push_back(1.0);
				verteices_data.push_back(0.0);
				break;

			case 3:
				verteices_data.push_back(size);
				verteices_data.push_back(size);
				verteices_data.push_back(0.0);

				verteices_data.push_back(1.0);
				verteices_data.push_back(0.0);
				break;

			case 4:
				verteices_data.push_back(-size);
				verteices_data.push_back(-size);
				verteices_data.push_back(0.0);

				verteices_data.push_back(0.0);
				verteices_data.push_back(1.0);
				break;

			case 5:
				verteices_data.push_back(size);
				verteices_data.push_back(-size);
				verteices_data.push_back(0.0);

				verteices_data.push_back(1.0);
				verteices_data.push_back(1.0);
				break;
			}

			verteices_data.push_back(start_time);
		}
	}
	


	enum attribute {
		pos, coord, start_time

		,stride
	};

	glGenVertexArrays(1, &m_Particle_VAO);
	glBindVertexArray(m_Particle_VAO);



	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verteices_data.size() * sizeof(float), &verteices_data[0], GL_STATIC_DRAW);
	


	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (stride - 1), &verteices_data[0]);

	glEnableVertexAttribArray(coord);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, sizeof(float)* (stride - 1), &verteices_data[3]);

	glEnableVertexAttribArray(start_time);
	glVertexAttribPointer(pos, 1, GL_FLOAT, GL_FALSE, sizeof(float)* (stride - 1), &verteices_data[5]);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Particle::Particle()
{
}

Particle::~Particle()
{
}
