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
	FrameData		Shadow;
	GLuint			Bloom_Shader;
	FrameData		post_process;

	GLuint			frameVAO;
	GLuint			frameVBO;


	GLuint			CompileShaders(std::string FileNameVS, std::string FileNameFS);
	bool			ReadShaderFile(std::string filename, std::string* target);
	void			AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	void			Initialize_ShadowMap(const unsigned int width, const unsigned int height);
	void			Initialize_PostProcessMap(const unsigned int width, const unsigned int height);

	void			Initialize(int width, int height);

	void			Render_ShadowMap(GLuint Shader, std::vector<Object*>Objects);
	void			Render_BloomMap(GLuint Shader, std::vector<Object*>Objects);
	void			Render_DefaultColor(GLuint Shader, std::vector<Object*>Objects);


public:
	Renderer(int width, int height);
	~Renderer();

	void			SetLight(Light* lights);

	float			GetAspect();
	GLuint			GetShader();
	void			DrawScene(std::vector<Object*>Objects);
};

