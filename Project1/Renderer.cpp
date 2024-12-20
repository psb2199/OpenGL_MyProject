#include "Renderer.h"
#include "Particle.h"

Renderer::Renderer(int width, int height, Importer* importer)
{
	Initialize(width, height);
	m_importer = importer;
}

Renderer::~Renderer()
{
}

GLuint Renderer::CompileShaders(std::string FileNameVS, std::string FileNameFS)
{
	std::cout << "Start Compile Shaders" << std::endl;
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		std::cout << "Error creating shader program" << std::endl;
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadShaderFile(FileNameVS, &vs)) {
		std::cout << "Error compiling vertex shader" << std::endl;
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadShaderFile(FileNameFS, &fs)) {
		std::cout << "Error compiling fragment shader" << std::endl;
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << FileNameVS << ", " << FileNameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << FileNameVS << ", " << FileNameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << FileNameVS << ", " << FileNameFS << " Shader compiling is done." << std::endl;

	return ShaderProgram;
}
void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}
bool Renderer::ReadShaderFile(std::string filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}
GLuint Renderer::GetShader(string type)
{
	if (type == "Basic") return Basic_Shader;
	else if (type == "Particle") return Particle_Shader;
}

float Renderer::GetAspect()
{
	return aspect;
}
void Renderer::SetLight(DirectionLight* lights)
{
	m_light = lights;
}
void Renderer::SetCamera(Camera* camera)
{
	m_Camera = camera;
}


void Renderer::Initialize(int width, int height)
{
	window_width = width;
	window_height = height;

	Basic_Shader = CompileShaders("BasicShader.vert", "BasicShader.frag");
	Particle_Shader = CompileShaders("ParticleShader.vert", "ParticleShader.frag");

	Enviroment_Shader = CompileShaders("EnviromentShader.vert", "EnviromentShader.frag");
	Initialize_EviromentVAO();

	Shadow_Shader = CompileShaders("ShadowShader.vert", "ShadowShader.frag");
	Initialize_ShadowMap(2048, 2048);

	Bloom_Shader = CompileShaders("BloomShader.vert", "BloomShader.frag");
	Initialize_BloomMap(window_width, window_height);

	PostProcess_Shader = CompileShaders("PostProcessShader.vert", "PostProcessShader.frag");
	Initialize_PostProcessMap(window_width, window_height);
}
void Renderer::Initialize_ShadowMap(const unsigned int width, const unsigned int height)
{
	Shadow.width = width;
	Shadow.height = height;

	glGenFramebuffers(1, &Shadow.FBO);

	glGenTextures(1, &Shadow.SceneID);
	glBindTexture(GL_TEXTURE_2D, Shadow.SceneID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, Shadow.FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Shadow.SceneID, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::Initialize_BloomMap(const unsigned int width, const unsigned int height)
{
	Bloom.width = width;
	Bloom.height = height;

	glGenFramebuffers(1, &Bloom.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, Bloom.FBO);

	glGenTextures(1, &Bloom.SceneID);
	glBindTexture(GL_TEXTURE_2D, Bloom.SceneID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Bloom.SceneID, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::Initialize_PostProcessMap(const unsigned int width, const unsigned int height)
{
	post_process.width = width;
	post_process.height = height;

	glGenFramebuffers(1, &post_process.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, post_process.FBO);

	glGenTextures(1, &post_process.SceneID);
	glBindTexture(GL_TEXTURE_2D, post_process.SceneID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_process.SceneID, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	float FrameVertecies[] = {
		-1.f, -1.f,   0.f, 0.f,  
		 1.f, -1.f,   1.f, 0.f,  
		-1.f,  1.f,   0.f, 1.f,

		 1.f, -1.f,   1.f, 0.f,
		 1.f,  1.f,   1.f, 1.f,
		-1.f,  1.f,   0.f, 1.f
	};

	glGenVertexArrays(1, &frameVAO);
	glGenBuffers(1, &frameVBO);

	glBindVertexArray(frameVAO);
	glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FrameVertecies), &FrameVertecies, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Renderer::Initialize_EviromentVAO()
{
	float skyboxSize = 100.f;
	float skyboxVertices[] = {
		// positions          
		-skyboxSize,  skyboxSize, -skyboxSize,
		-skyboxSize, -skyboxSize, -skyboxSize,
		 skyboxSize, -skyboxSize, -skyboxSize,
		 skyboxSize, -skyboxSize, -skyboxSize,
		 skyboxSize,  skyboxSize, -skyboxSize,
		-skyboxSize,  skyboxSize, -skyboxSize,
		 			     
		-skyboxSize, -skyboxSize,  skyboxSize,
		-skyboxSize, -skyboxSize, -skyboxSize,
		-skyboxSize,  skyboxSize, -skyboxSize,
		-skyboxSize,  skyboxSize, -skyboxSize,
		-skyboxSize,  skyboxSize,  skyboxSize,
		-skyboxSize, -skyboxSize,  skyboxSize,
		 			     
		 skyboxSize, -skyboxSize, -skyboxSize,
		 skyboxSize, -skyboxSize,  skyboxSize,
		 skyboxSize,  skyboxSize,  skyboxSize,
		 skyboxSize,  skyboxSize,  skyboxSize,
		 skyboxSize,  skyboxSize, -skyboxSize,
		 skyboxSize, -skyboxSize, -skyboxSize,
					     
		-skyboxSize, -skyboxSize,  skyboxSize,
		-skyboxSize,  skyboxSize,  skyboxSize,
		 skyboxSize,  skyboxSize,  skyboxSize,
		 skyboxSize,  skyboxSize,  skyboxSize,
		 skyboxSize, -skyboxSize,  skyboxSize,
		-skyboxSize, -skyboxSize,  skyboxSize,
					     
		-skyboxSize,  skyboxSize, -skyboxSize,
		 skyboxSize,  skyboxSize, -skyboxSize,
		 skyboxSize,  skyboxSize,  skyboxSize,
		 skyboxSize,  skyboxSize,  skyboxSize,
		-skyboxSize,  skyboxSize,  skyboxSize,
		-skyboxSize,  skyboxSize, -skyboxSize,
					     
		-skyboxSize, -skyboxSize, -skyboxSize,
		-skyboxSize, -skyboxSize,  skyboxSize,
		 skyboxSize, -skyboxSize, -skyboxSize,
		 skyboxSize, -skyboxSize, -skyboxSize,
		-skyboxSize, -skyboxSize,  skyboxSize,
		 skyboxSize, -skyboxSize,  skyboxSize
	};

	glGenVertexArrays(1, &EviromentVAO);
	glGenBuffers(1, &EviromentVBO);

	glBindVertexArray(EviromentVAO);

	glBindBuffer(GL_ARRAY_BUFFER, EviromentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(0);
}

void Renderer::GetObjectShaderAttributes(GLuint shader, Object* obj)
{
	glm::vec3 location = obj->GetLocation();
	glm::vec3 rotation = obj->GetRotation();
	glm::quat quaternionRotation = glm::quat(glm::radians(rotation));

	float halfheight = obj->GetMesh()->min_location.y;

	glm::mat4 transfom_Matrix = glm::mat4(1.0f);
	transfom_Matrix = glm::translate(transfom_Matrix, location + glm::vec3(0.0, halfheight, 0.0));
	transfom_Matrix = glm::scale(transfom_Matrix, obj->GetScale());
	transfom_Matrix = glm::translate(transfom_Matrix, -glm::vec3(0.0, halfheight, 0.0));
	transfom_Matrix *= glm::toMat4(quaternionRotation);

	unsigned int ObjectTransform = glGetUniformLocation(shader, "transform");
	glUniformMatrix4fv(ObjectTransform, 1, GL_FALSE, glm::value_ptr(transfom_Matrix));

	glUniform3f(glGetUniformLocation(shader, "actor_location"), location.x, location.y, location.z);

	glm::mat4 normal_Matrix = glm::mat4(1.0f);
	normal_Matrix *= glm::toMat4(quaternionRotation);
	glUniformMatrix4fv(glGetUniformLocation(shader, "normal_transform"), 1, GL_FALSE, glm::value_ptr(normal_Matrix));
	
	glUniform1f(glGetUniformLocation(shader, "elapsedTime"), obj->GetElapsedTime());

	if (obj->GetObjectType(obj) == type_Particle)
	{
		Particle* asParticle = dynamic_cast<Particle*>(obj);
		asParticle->DoParticleUniform(shader);
	}
}


void Renderer::DrawScene(std::vector<Object*>&Objects)
{
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);
	aspect = (float)window_width / (float)window_height;

	Render_ShadowMap(Shadow_Shader, Objects);
	Render_BloomMap(Bloom_Shader, Objects);

	glBindFramebuffer(GL_FRAMEBUFFER, post_process.FBO);
	Render_Enviroment(Enviroment_Shader);
	Render_DefaultColor(Objects);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Render_PostProcessMap(PostProcess_Shader);
}

void Renderer::Render_ShadowMap(GLuint Shader, std::vector<Object*> Objects)
{
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, Shadow.FBO);
	glViewport(0, 0, Shadow.width, Shadow.height);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(Shader);

	m_light->LightWorks(Shader);


	// 객체별로 변환 행렬 설정 및 렌더링
	for (Object* object : Objects)
	{
		GetObjectShaderAttributes(Shader, object);

		if (!object->setting.cast_shadow) continue;

		glBindVertexArray(object->GetMesh()->VAO);
		glDrawArrays(GL_TRIANGLES, 0, object->GetMesh()->polygon_count * 3);
		glBindVertexArray(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window_width, window_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}
void Renderer::Render_DefaultColor(std::vector<Object*> Objects)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT); // 프레임 클리어 추가

	for (Object* object : Objects)
	{
		if (!object->setting.EnableRendering) continue;



		prev_Material* obj_material = object->GetMaterial();
		GLuint Shader = GetShader(obj_material->shader_name);
		if (obj_material->shader_name == "Particle")
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glUseProgram(Shader);
		CameraMat camera_mat = m_Camera->DoWorking(Shader, aspect);

		if (!m_Camera->isCollisionBoxInFrustum(m_Camera->extractFrustumPlanes(camera_mat.projection * camera_mat.view), object->GetCollisionRange())) {
			continue;
		}

		//object->printObject(object);

		m_light->LightWorks(Shader);

		GLuint ul_Depth = glGetUniformLocation(Shader, "u_DepthMap");
		glUniform1i(ul_Depth, 4);  
		glActiveTexture(GL_TEXTURE0 + 4);  
		glBindTexture(GL_TEXTURE_2D, Shadow.SceneID);  

		GLuint ul_ShadowMapSize = glGetUniformLocation(Shader, "u_ShadowMapSize");
		glUniform1f(ul_ShadowMapSize, Shadow.width);

		GLuint ul_enviroment = glGetUniformLocation(Shader, "u_enviroment");
		glUniform1i(ul_enviroment, 5);
		glActiveTexture(GL_TEXTURE0 + 5);  
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_importer->GetEnviromentMaterial());

		glm::vec3 camera_dir = m_Camera->GetCameraDirection();

		GetObjectShaderAttributes(Shader, object);

		GLuint ul_BaseColor = glGetUniformLocation(Shader, "u_BaseColor");
		glUniform1i(ul_BaseColor, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, obj_material->BaseColorID);

		GLuint ul_NormalMap = glGetUniformLocation(Shader, "u_NormalMap");
		glUniform1i(ul_NormalMap, 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, obj_material->NormalMapID);

		GLuint ul_ARM = glGetUniformLocation(Shader, "u_ARM");
		glUniform1i(ul_ARM, 3);
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, obj_material->AoRoughnessMetallicID);

		GLuint ul_cast_shadow = glGetUniformLocation(Shader, "u_cast_shadow");
		glUniform1i(ul_cast_shadow, object->setting.cast_shadow ? 0 : 1);

		if(object->GetObjectType(object) == type_Particle) glDepthMask(GL_FALSE);
		if(object->setting.EnalbeTwoFace) glDisable(GL_CULL_FACE);

		glBindVertexArray(object->GetMesh()->VAO);
		glDrawArrays(GL_TRIANGLES, 0, object->GetMesh()->polygon_count * 3);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
	}
}



void Renderer::Render_BloomMap(GLuint Shader, std::vector<Object*> Objects)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Bloom.FBO);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT); 

	glUseProgram(Shader);
	m_Camera->DoWorking(Shader, aspect);



	// 객체별로 변환 행렬 설정 및 렌더링
	for (Object* object : Objects)
	{

		CameraMat camera_mat = m_Camera->DoWorking(Shader, aspect);

		if (!m_Camera->isCollisionBoxInFrustum(m_Camera->extractFrustumPlanes(camera_mat.projection * camera_mat.view), object->GetCollisionRange())) {
			continue;
		}

		VertexData* mesh = object->GetMesh();
		GetObjectShaderAttributes(Shader, object);

		GLuint ul_Emissive = glGetUniformLocation(Shader, "u_Emissive");
		glUniform1i(ul_Emissive, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, object->GetMaterial()->EmissiveID);

		glBindVertexArray(mesh->VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh->polygon_count * 3);
		glBindVertexArray(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window_width, window_height);

	glDisable(GL_DEPTH_TEST);
}
void Renderer::Render_Enviroment(GLuint Shader)
{
	glDepthMask(GL_FALSE);

	glUseProgram(Shader);

	m_Camera->DoWorking(Shader, aspect);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_importer->GetEnviromentMaterial());
	GLuint ul_enviroment = glGetUniformLocation(Shader, "u_enviroment");
	glUniform1i(ul_enviroment, 0); 

	glBindVertexArray(EviromentVAO); 
	glDrawArrays(GL_TRIANGLES, 0, 36); 

	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}
void Renderer::Render_PostProcessMap(GLuint Shader)
{
	glUseProgram(Shader);

	glUniform2f(glGetUniformLocation(Shader, "ViewSize"), window_width, window_height);

	GLuint ul_Bloom = glGetUniformLocation(Shader, "u_Bloom");
	glUniform1i(ul_Bloom, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Bloom.SceneID);

	GLuint ul_PostProcess = glGetUniformLocation(Shader, "u_PostProcess");
	glUniform1i(ul_PostProcess, 1);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, post_process.SceneID);

	glBindVertexArray(frameVAO); // 정점 배열 객체 바인딩
	glDrawArrays(GL_TRIANGLES, 0, 6); // 화면에 텍스처 그리기
	glBindVertexArray(0);
}



