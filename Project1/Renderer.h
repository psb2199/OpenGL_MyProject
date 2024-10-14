#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "OpenGL.h"
#include "ObjectManager.h"
#include "Importer_obj.h"

#include "Light.h"
#include "Camera.h"


class Renderer
{	
	int				window_width;
	int				window_height;
	float			aspect;
	Importer_obj*	m_importer;

	Light*			m_light;

	Camera*			m_Camera;

	GLuint			Basic_Shader;
	GLuint			Shadow_Shader;
	GLuint			Bloom_Shader;
	GLuint			Enviroment_Shader;

	FrameData		Shadow;
	FrameData		post_process;

	GLuint			EviromentVAO;
	GLuint			EviromentVBO;
	GLuint			frameVAO;
	GLuint			frameVBO;
	unsigned int	rbo;


	GLuint			CompileShaders(std::string FileNameVS, std::string FileNameFS);
	bool			ReadShaderFile(std::string filename, std::string* target);
	void			AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);


	void			Initialize(int width, int height);
	void			Initialize_ShadowMap(const unsigned int width, const unsigned int height);
	void			Initialize_PostProcessMap(const unsigned int width, const unsigned int height);
	void			Initialize_EviromentVAO();

	void			Render_ShadowMap(GLuint Shader, std::vector<Object*>Objects);
	void			Render_PostProcessMap(GLuint Shader, std::vector<Object*>Objects);
	void			Render_DefaultColor(GLuint Shader, std::vector<Object*>Objects);
	void			Render_Enviroment(GLuint Shader);


public:
	Renderer(int width, int height, Importer_obj* importer);
	~Renderer();

	void			SetLight(Light* lights);
	void			SetCamera(Camera* camera);


	float			GetAspect();
	GLuint			GetShader();
	void			DrawScene(std::vector<Object*>Objects);
};

