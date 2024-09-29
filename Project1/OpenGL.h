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
enum ObjFile { test, MAX_COUNT };

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

	GLuint VAO;
	GLuint VBO;
	GLuint texCoordVBO;
	GLuint normalVBO;
	GLuint EBO;

	vector<glm::vec3> vertexs;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;

	vector<glm::vec3> faceIndices;
	vector<glm::vec3> texCoordIndices;
	vector<glm::vec3> normalIndices;
};