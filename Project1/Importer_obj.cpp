#include "Importer_obj.h"

Importer_obj::Importer_obj()
{
	Initialize();
}

Importer_obj::~Importer_obj()
{
}

void Importer_obj::Initialize()
{
	ReadObj("objs/Test.obj");
	ReadObj("objs/GravityBox.obj");
	ReadObj("objs/Base.obj");
	//ReadObj("Male.obj");

	LoadTexture("textures/GravityBox_BaseColor.png", GL_NEAREST);
}

void Importer_obj::DeBugVertexData(VertexData* VD)
{
	int count{ 0 };
	for (auto& v : VD->vertexs) {
		//std::cout << v.x << " " << v.y << " " << v.z << std::endl;
		
		
	}

	std::cout << std::endl;

	for (auto& v : VD->normals) {
		//std::cout << v.x << " " << v.y << " " << v.z << std::endl;
		count++;
		
	}
	std::cout << std::endl;

	for (auto& v : VD->texCoords) {
		//std::cout << v.x << " " << v.y << std::endl;
	}

	for (auto& v : VD->normalIndices) {
		std::cout << v.x << " " << v.y << " " << v.z << std::endl;
	}

	std::cout << count << endl;
}

void Importer_obj::ReadObj(const string filePath) {
	ifstream objFile(filePath);

	if (!objFile.is_open()) {
		cerr << "Error: Unable to open file " << filePath << endl;
		return;
	}
	else {
		cout << "File Load Success: " << filePath << endl;
	}

	

	VertexData* newVertexData = new VertexData;
	newVertexData->filename = removeSubstring(filePath, "objs/");

	string line;

	// 1. 파일에서 버텍스, 텍스처 좌표, 노멀 및 인덱스 데이터 읽기
	while (getline(objFile, line)) {
		if (line.empty()) continue;  // 빈 줄 무시

		stringstream ss(line);
		string type;
		ss >> type;

		if (type == "v") {
			// 버텍스 좌표 읽기
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			newVertexData->vertexs.push_back(vertex);

		}
		else if (type == "vt") {
			// 텍스처 좌표 읽기
			glm::vec2 texCoord;
			ss >> texCoord.x >> texCoord.y;
			newVertexData->texCoords.push_back(texCoord);

		}
		else if (type == "vn") {
			// 노멀 좌표 읽기
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			newVertexData->normals.push_back(normal);

		}
		else if (type == "f") {
			char dummy;

			glm::vec3 faceIndex;
			glm::vec3 texCoordIndex;
			glm::vec3 normalIndex;

			ss >> faceIndex.x >> dummy >> texCoordIndex.x >> dummy >> normalIndex.x
			   >> faceIndex.y >> dummy >> texCoordIndex.y >> dummy >> normalIndex.y
			   >> faceIndex.z >> dummy >> texCoordIndex.z >> dummy >> normalIndex.z;

			newVertexData->faceIndices.push_back(faceIndex);
			newVertexData->texCoordIndices.push_back(texCoordIndex);
			newVertexData->normalIndices.push_back(normalIndex);

			newVertexData->polygon_count++;
		}
	}


	//DeBugVertexData(newVertexData);
	// 메쉬 설정 (버텍스, 텍스처, 노멀 좌표 및 인덱스 데이터로)
	setupMesh(newVertexData);

	// VertexBuffer 리스트에 추가
	VertexBuffers.push_back(newVertexData);
}

void Importer_obj::LoadTexture(const char* filepath, GLuint samplingMethod)
{
	TextureData* newtexture = new TextureData;
	newtexture->filename = removeSubstring(filepath, "textures/");
	//Load Png

	std::vector<unsigned char> image;

	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filepath);

	if (error != 0)
	{
		cout << "PNG image loading failed:" << filepath << endl;
	}

	glGenTextures(1, &newtexture->textureID);
	glBindTexture(GL_TEXTURE_2D, newtexture->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);
	
	

	Textures.push_back(newtexture);
	cout << "PNG image loading Success:" << filepath << endl;
}

TextureData* Importer_obj::FindTexture(std::string filename)
{
	for (std::vector<TextureData*>::iterator itr = Textures.begin(); itr != Textures.end(); ++itr)
	{
		if ((*itr)->filename == filename) return (*itr);
	}

	std::cout << "Can not find" << filename << "texture Asset." << std::endl;
	return nullptr;
}


VertexData* Importer_obj::FindMesh(std::string filename)
{
	for (std::vector<VertexData*>::iterator itr = VertexBuffers.begin(); itr != VertexBuffers.end(); ++itr)
	{
		if ((*itr)->filename == filename) return (*itr);
	}

	std::cout << "Can not find" << filename << "Mesh Asset." << std::endl;
	return nullptr;
}


void Importer_obj::setupMesh(VertexData* VD) {
	// VAO, VBO, EBO 생성
	glGenVertexArrays(1, &VD->VAO);

	glGenBuffers(1, &VD->VBO);
	glGenBuffers(1, &VD->texCoordVBO);
	glGenBuffers(1, &VD->normalVBO);

	// VAO 바인딩
	glBindVertexArray(VD->VAO);

	vector<glm::vec3> re_vertexs;
	vector<glm::vec2> re_texcoords;
	vector<glm::vec3> re_normals;
	std::vector<GLuint> indices;

	for (size_t i = 0; i < VD->faceIndices.size(); ++i) {
		re_vertexs.push_back(VD->vertexs[VD->faceIndices[i].x - 1]);
		re_vertexs.push_back(VD->vertexs[VD->faceIndices[i].y - 1]);
		re_vertexs.push_back(VD->vertexs[VD->faceIndices[i].z - 1]);

		re_texcoords.push_back(VD->texCoords[VD->texCoordIndices[i].x - 1]);
		re_texcoords.push_back(VD->texCoords[VD->texCoordIndices[i].y - 1]);
		re_texcoords.push_back(VD->texCoords[VD->texCoordIndices[i].z - 1]);

		re_normals.push_back(VD->normals[VD->normalIndices[i].x - 1]);
		re_normals.push_back(VD->normals[VD->normalIndices[i].y - 1]);
		re_normals.push_back(VD->normals[VD->normalIndices[i].z - 1]);
	}

	// VBO에 버텍스 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->VBO);
	glBufferData(GL_ARRAY_BUFFER, re_vertexs.size() * sizeof(glm::vec3), &re_vertexs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // 위치 속성 인덱스
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// VBO에 텍스처 좌표 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, re_texcoords.size() * sizeof(glm::vec2), &re_texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // 텍스처 좌표 속성 인덱스
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	// VBO에 노멀 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->normalVBO);
	glBufferData(GL_ARRAY_BUFFER, re_normals.size() * sizeof(glm::vec3), &re_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // 노멀 속성 인덱스
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	
	// VAO 해제
	glBindVertexArray(0);
}

std::string Importer_obj::removeSubstring(const std::string& str, const std::string& toRemove) {

	std::size_t pos = str.find(toRemove);

	if (pos != std::string::npos) {
		return str.substr(pos + toRemove.length());
	}

	return str;
}



