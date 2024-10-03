#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <iostream>
#include <vector>
using namespace std;

enum press { w, a, s, d, KEY_COUNT};

struct vector3 
{ 
	float x, y, z; 

	void print_vec3() 
	{
		cout << " x:" << x << " y:" << y << " z:" << z << endl;
	}
};

struct VertexData {
	std::string filename;

	unsigned int polygon_count{ 0 };

	GLuint VAO;
	GLuint VBO;
	GLuint texCoordVBO;
	GLuint normalVBO;

	vector<glm::vec3> vertexs;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;

	vector<glm::vec3> faceIndices;
	vector<glm::vec3> texCoordIndices;
	vector<glm::vec3> normalIndices;
};

struct TextureData {

	std::string filename;
	GLuint textureID;
};

struct Material {
	std::string material_name;

	GLuint BaseColorID;
	GLuint NormalMapID;
};