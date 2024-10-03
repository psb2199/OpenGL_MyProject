#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "OpenGL.h"
#include "LoadPng.h"


class Importer_obj
{
	//GLuint VAO, VBO, EBO;

	
	//VertexBuffer VertexBuffers[ObjFile(MAX_COUNT)];

	std::vector<VertexData*> VertexBuffers;
	std::vector<Material*> Materials;

	void Initialize();

	void DeBugVertexData(VertexData* VD);
	std::string removeSubstring(const std::string& str, const std::string& toRemove);

	void CalculateTangentBitangent(
		const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3,
		const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3,
		glm::vec3& tangent, glm::vec3& bitangent);

	void MakeMaterial(const std::string MaterialName, GLuint BaseColor, GLuint NormalMap, GLuint Emissive);
	GLuint LoadTexture(const char* filepath);
public:
	Material* GetMaterial(std::string filename);
	void ReadObj(const string filePath);
	VertexData* FindMesh(std::string filename);
	void setupMesh(VertexData* VB);

	Importer_obj();
	~Importer_obj();
};

