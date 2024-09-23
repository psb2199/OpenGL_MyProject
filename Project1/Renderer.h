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

	void Initialize(int width, int height);
	bool ReadFile(char* filename, std::string* target);

	GLuint CompileShaders(char* FileNameVS, char* FileNameFS);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
public:
	Renderer(int width, int height);
	~Renderer();
};

