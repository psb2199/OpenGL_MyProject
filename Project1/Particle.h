#pragma once
#include "OpenGL.h"

class Particle
{
	GLuint m_Particle_VAO;

	void CreateParticleObject(int particle_count);

public:
	Particle();
	~Particle();
};

