#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "OpenGL.h"



class Importer_obj
{
	//GLuint VAO, VBO, EBO;

	
	VertexBuffer VertexBuffers[ObjFile(MAX_COUNT)];

	void Initialize();

public:
	void ReadObj(const string filePath, VertexBuffer VB);
	void setupMesh(VertexBuffer VB, const vector<glm::vec3>& vertices, const vector<unsigned int>& indices);

	Importer_obj();
	~Importer_obj();
};

