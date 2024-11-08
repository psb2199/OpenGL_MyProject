#pragma once

#include <fstream>
#include <sstream>

#include "OpenGL.h"
#include "LoadPng.h"
#include "ParticleMaker.h"


class Importer
{
	//GLuint VAO, VBO, EBO;

	
	//VertexBuffer VertexBuffers[ObjFile(MAX_COUNT)];

	std::vector<VertexData*> VertexBuffers;
	std::vector<Material*> Materials;
	GLuint enviroment_Material;

	void Initialize();

	void DeBugVertexData(VertexData* VD);
	std::string removeSubstring(const std::string& str, const std::string& toRemove);

	void CalculateTangentBitangent(
		const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3,
		const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3,
		glm::vec3& tangent, glm::vec3& bitangent);

	void MakeMaterial(const std::string MaterialName, std::string shaderName, GLuint BaseColor, GLuint NormalMap, GLuint Emissive, GLuint ARM);

public:
	GLuint LoadTexture(const char* filepath);
	GLuint LoadEnviromentTextures(std::vector<string> filepathes);

	Material* GetMaterial(std::string filename);
	GLuint GetEnviromentMaterial();

	void ReadObj(const string filePath);
	VertexData* FindMesh(std::string filename);
	void setupMesh(VertexData* VB);

	Importer();
	~Importer();
};

