#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "OpenGL.h"
#include "ObjectManager.h"
#include "Importer_obj.h"

#include "DirectionLight.h"
#include "Camera.h"


class Renderer
{	
	int				window_width;
	int				window_height;
	float			aspect;
	Importer_obj*	m_importer;

	DirectionLight*			m_light;

	Camera*			m_Camera;

	GLuint			Basic_Shader;
	GLuint			Shadow_Shader;
	GLuint			Bloom_Shader;
	GLuint			PostProcess_Shader;
	GLuint			Enviroment_Shader;

	FrameData		Shadow;
	FrameData		Bloom;
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
	void			Initialize_BloomMap(const unsigned int width, const unsigned int height);
	void			Initialize_PostProcessMap(const unsigned int width, const unsigned int height);
	void			Initialize_EviromentVAO();

	void			Render_ShadowMap(GLuint Shader, std::vector<Object*>Objects);
	void			Render_BloomMap(GLuint Shader, std::vector<Object*>Objects);
	void			Render_DefaultColor(GLuint Shader, std::vector<Object*>Objects);
	void			Render_Enviroment(GLuint Shader);
	void			Render_PostProcessMap(GLuint Shader);

	void			GetObjectTrnasformMatrix(GLuint shader, Object* obj);

public:
	Renderer(int width, int height, Importer_obj* importer);
	~Renderer();

	void			SetLight(DirectionLight* lights);
	void			SetCamera(Camera* camera);


	float			GetAspect();
	GLuint			GetShader();
	void			DrawScene(std::vector<Object*>Objects);
};

