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
	std::vector<GLuint*> Textures;

	void Initialize();

	void DeBugVertexData(VertexData* VD);
	std::string removeSubstring(const std::string& str, const std::string& toRemove);
public:
	void LoadTexture(const char* filepath, GLuint samplingMethod);
	void ReadObj(const string filePath);
	VertexData* FindMesh(std::string filename);
	void setupMesh(VertexData* VB);

	Importer_obj();
	~Importer_obj();
};

