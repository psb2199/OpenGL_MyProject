#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "OpenGL.h"
#include "ObjectManager.h"


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

public:
	Renderer(int width, int height);
	~Renderer();

	GLuint GetShader();
	void DrawScene(std::vector<Object*>Objects);
};

