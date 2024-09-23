#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>


class Renderer
{
	int window_width;
	int window_height;

	GLuint Basic_Shader;
	GLuint Basic_VBO;

	void Initialize(int width, int height);

	GLuint CompileShaders(std::string FileNameVS, std::string FileNameFS);
	bool ReadShaderFile(std::string filename, std::string* target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	void CreateVertexBufferObjects();

public:
	Renderer(int width, int height);
	~Renderer();

	void DrawScene();
};

