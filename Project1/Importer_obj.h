#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "OpenGL.h"

class Importer_obj
{
	GLuint VAO, VBO, EBO;

public:
	void ReadObj(const string& filePath);
	void setupMesh(const vector<glm::vec3>& vertices, const vector<unsigned int>& indices);

	Importer_obj();
	~Importer_obj();
};

