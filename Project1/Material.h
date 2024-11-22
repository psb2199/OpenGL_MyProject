#pragma once
#include "OpenGL.h"

#include "LoadPng.h"


class Material
{
	std::string material_name;

	GLuint BaseColor_TextureID;
	GLuint Normal_TextureID;
	GLuint ARM_TextureID;
	GLuint Emissive_TextureID;

	GLuint Shader;

public:
	Material* MakeMaterial(
		std::string material_name,
		GLuint shader,
		GLuint baseColor,
		GLuint normal,
		GLuint arm,
		GLuint emissive
		);

	Material* GetMaterial(std::string material_name);

	Material();
	~Material();
};

