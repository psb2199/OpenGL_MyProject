#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/quaternion.hpp>
#include <gl/glm/gtx/quaternion.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <random>

#include <iostream>
#include <vector>
#include <array>
using namespace std;

enum press { w, a, s, d, KEY_COUNT};

enum objtype
{
	type_Player,
	type_Base,
	type_Coin,
	type_Particle,

	P_PopCoin
};

struct VertexData {
	std::string filename;

	unsigned int polygon_count{ 0 };

	GLuint VAO;
	GLuint VBO;
	GLuint texCoordVBO;
	GLuint normalVBO;
	GLuint tangentVBO;
	GLuint bitangentVBO;

	vector<glm::vec3> vertexs;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;

	vector<glm::vec3> faceIndices;
	vector<glm::vec3> texCoordIndices;
	vector<glm::vec3> normalIndices;

	glm::vec3 min_location = glm::vec3(0.0);
	glm::vec3 max_location = glm::vec3(0.0);
};

struct TextureData {

	std::string filename;
	unsigned int textureID;
};

struct FrameData {

	GLuint FBO;
	GLuint SceneID;

	unsigned int width;
	unsigned int height;
};

struct Material {
	std::string material_name;

	std::string shader_name;

	GLuint BaseColorID;
	GLuint NormalMapID;
	GLuint AoRoughnessMetallicID;
	GLuint EmissiveID;
};

struct CollisionBox {
	glm::vec3 min;
	glm::vec3 max;

};

static void printVector3(glm::vec3 vec)
{
	cout << "x:" << vec.x << "  y:" << vec.y << "  z:" << vec.z << endl;
}

static int GetRandint(int min, int max) {
	
	static std::random_device rd;
	static std::mt19937 gen(rd());  
	std::uniform_int_distribution<> dist(min, max);

	return dist(gen);  
}