#include "Renderer.h"


Renderer::Renderer(int width, int height)
{
	Initialize(width, height);
}

Renderer::~Renderer()
{
}

void Renderer::SetLight(Light* lights)
{
	m_light = lights;
}

void Renderer::Initialize(int width, int height)
{
	window_width = width;
	window_height = height;

	Basic_Shader = CompileShaders("BasicShader.vs", "BasicShader.fs");

	//ShadowMapping();
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

void Renderer::ShadowMapping()
{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	// 깊이 맵 텍스처 생성
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// 깊이만 렌더링하는 프레임버퍼에 텍스처 부착
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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


float Renderer::GetAspect()
{
	return aspect;
}

GLuint Renderer::GetShader()
{
	return Basic_Shader;
}

void Renderer::DrawScene(std::vector<Object*>Objects)
{
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);
	aspect = (float)window_width / (float)window_height;

	glUseProgram(Basic_Shader);
	
	for (std::vector<Object*>::iterator itr = Objects.begin(); itr != Objects.end(); ++itr) 
	{
		// light =================================================================
		unsigned int lightPosLocation = glGetUniformLocation(Basic_Shader, "lightPos");
		glm::vec3 light_location = m_light->GetLocation();
		glUniform3f(lightPosLocation, light_location.x, light_location.y, light_location.z);

		unsigned int lightColorLocation = glGetUniformLocation(Basic_Shader, "lightColor");
		glm::vec3 light_color = m_light->GetLightColor();
		glUniform3f(lightColorLocation, light_color.r, light_color.g, light_color.b);

		unsigned int lightDistanceLocation = glGetUniformLocation(Basic_Shader, "lightDistance");
		glUniform1f(lightDistanceLocation, m_light->GetLightDistance());
		//========================================================================

		vector3 location = (*itr)->GetLocation();
		vector3 rotation = (*itr)->GetRotation();
		
		glm::mat4 transfom_Matrix = glm::mat4(1.0f);

		transfom_Matrix = glm::translate(transfom_Matrix, glm::vec3(location.x, location.y, location.z));
		
		transfom_Matrix = glm::rotate(transfom_Matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		transfom_Matrix = glm::rotate(transfom_Matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		transfom_Matrix = glm::rotate(transfom_Matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		
		transfom_Matrix = glm::scale(transfom_Matrix, glm::vec3(1.0, 1.0, 1.0));
		
		unsigned int ObjectTransform = glGetUniformLocation(Basic_Shader, "transform");
		glUniformMatrix4fv(ObjectTransform, 1, GL_FALSE, glm::value_ptr(transfom_Matrix));

		GLuint ul_BaseColor = glGetUniformLocation(Basic_Shader, "u_BaseColor");
		glUniform1i(ul_BaseColor, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, (*itr)->GetMaterial()->BaseColorID);

		GLuint ul_NormalMap = glGetUniformLocation(Basic_Shader, "u_NormalMap");
		glUniform1i(ul_NormalMap, 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, (*itr)->GetMaterial()->NormalMapID);

		GLuint ul_Emissive = glGetUniformLocation(Basic_Shader, "u_Emissive");
		glUniform1i(ul_Emissive, 2);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, (*itr)->GetMaterial()->EmissiveID);

		glBindVertexArray((*itr)->GetMesh()->VAO);
		glDrawArrays(GL_TRIANGLES, 0, (*itr)->GetMesh()->polygon_count * 3);
		glBindVertexArray(0);
	}



}
