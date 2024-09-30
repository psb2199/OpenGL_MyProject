#include "Renderer.h"


Renderer::Renderer(int width, int height)
{
	Initialize(width, height);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int width, int height)
{
	window_width = width;
	window_height = height;

	Basic_Shader = CompileShaders("BasicShader.vs", "BasicShader.fs");
	CreateVertexBufferObjects();
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

void Renderer::CreateVertexBufferObjects()
{
	float cubesize = 0.5;

	float rect[] =
	{
		//앞면
	-cubesize,cubesize,cubesize,	1,0,0,
	cubesize,-cubesize,cubesize,	0,1,0,
	-cubesize,-cubesize,cubesize,	0,0,1,

	cubesize,-cubesize,cubesize,1,0,0,
	-cubesize,cubesize,cubesize,0,1,0,
	cubesize,cubesize,cubesize,	0,0,1,

	//윗면
	-cubesize,cubesize,-cubesize,1,0,0,
	cubesize,cubesize,cubesize,	 0,1,0,
	-cubesize,cubesize,cubesize, 0,0,1,

	cubesize,cubesize,-cubesize,1,0,0,
	cubesize,cubesize,cubesize,	0,1,0,
	-cubesize,cubesize,-cubesize, 0,0,1,

	//아랫면
	cubesize,-cubesize,cubesize,	1,0,0,
	-cubesize,-cubesize,-cubesize,	0,1,0,
	-cubesize,-cubesize,cubesize,	0,0,1,

	-cubesize,-cubesize,-cubesize,	1,0,0,
	cubesize,-cubesize,cubesize,	0,1,0,
	cubesize,-cubesize,-cubesize,	0,0,1,

	//뒷면
	cubesize,-cubesize,-cubesize,	1,0,0,
	-cubesize,cubesize,-cubesize,	0,1,0,
	-cubesize,-cubesize,-cubesize,	0,0,1,

	-cubesize,cubesize,-cubesize,	1,0,0,
	cubesize,-cubesize,-cubesize,	0,1,0,
	cubesize,cubesize,-cubesize,	0,0,1,

	//오른면
	cubesize,cubesize,cubesize,		1,0,0,
	cubesize,-cubesize,-cubesize,	0,1,0,
	cubesize,-cubesize,cubesize,	0,0,1,

	cubesize,cubesize,cubesize,		1,0,0,
	cubesize,cubesize,-cubesize,	0,1,0,
	cubesize,-cubesize,-cubesize,	0,0,1,

	//외면
	-cubesize,-cubesize,-cubesize,	1,0,0,
	 -cubesize,cubesize,cubesize,	0,1,0,
	-cubesize,-cubesize,cubesize,	0,0,1,

	-cubesize,cubesize,-cubesize,	1,0,0,
	-cubesize,cubesize,cubesize,	0,1,0,
	-cubesize,-cubesize,-cubesize,	0,0,1
	};

	float data_float_count = 3 + 3; //(x,y,z,   r,g,b)

	glGenBuffers(1, &Basic_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Basic_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * data_float_count, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * data_float_count, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

GLuint Renderer::GetShader()
{
	return Basic_Shader;
}

void Renderer::DrawScene(std::vector<Object*>Objects)
{
	glUseProgram(Basic_Shader);
	
	for (std::vector<Object*>::iterator itr = Objects.begin(); itr != Objects.end(); ++itr) 
	{
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


		glBindVertexArray((*itr)->GetMesh()->VAO);
		//glDrawElements(GL_TRIANGLES, 5432*3, GL_UNSIGNED_INT, 0); // 인덱스 기반 그리기
		glDrawArrays(GL_TRIANGLES, 0, (*itr)->GetMesh()->polygon_count * 3);
		glBindVertexArray(0);
	}



}
