#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "OpenGL.h"
#include "ObjectManager.h"
#include "Importer_obj.h"

#include "Light.h"


class Renderer
{	
	int				window_width;
	int				window_height;
	float			aspect;

	Light*			m_light;

	GLuint			Basic_Shader;
	GLuint			Shadow_Shader;

	GLuint			depthMapFBO;
	GLuint			depthMap;
	void			InitializeDepthMap();

	void			Initialize(int width, int height);

	GLuint			CompileShaders(std::string FileNameVS, std::string FileNameFS);
	bool			ReadShaderFile(std::string filename, std::string* target);
	void			AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

public:
	Renderer(int width, int height);
	~Renderer();

	void			SetLight(Light* lights);

	float			GetAspect();
	GLuint			GetShader();
	void			DrawScene(std::vector<Object*>Objects);
};

