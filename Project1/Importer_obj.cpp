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
	ReadObj("objs/Male.obj");

	MakeMaterial
	(
		"M_GravityBox",
		LoadTexture("textures/GravityBox_BaseColor.png"),
		LoadTexture("textures/GravityBox_Normal.png"),
		LoadTexture("textures/GravityBox_Emissive.png")
	);

	MakeMaterial
	(
		"M_Grass",
		LoadTexture("textures/Grass_BaseColor.png"),
		LoadTexture("textures/Grass_Normal.png"),
		NULL
	);

	MakeMaterial
	(
		"M_Male",
		LoadTexture("textures/Male_BaseColor.png"),
		LoadTexture("textures/Male_Normal.png"),
		LoadTexture("textures/Male_Emissive.png")
	);
}


GLuint Importer_obj::LoadTexture(const char* filepath)
{
	//Load Png
	GLuint texture;

	std::vector<unsigned char> image;

	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filepath);

	if (error != 0)
	{
		cout << "PNG image loading failed:" << filepath << endl;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	cout << "PNG image loading Success:" << filepath << endl;

	return texture;
}
Material* Importer_obj::GetMaterial(std::string filename)
{
	for (auto& v : Materials) {
		
		if (v->material_name == filename) return v;
	}

	std::cout << "Can not find" << filename << "Material Asset." << std::endl;
	return nullptr;
}
void Importer_obj::MakeMaterial(const std::string MaterialName, GLuint BaseColor, GLuint NormalMap, GLuint Emissive)
{
	Material* newMaterial = new Material;
	newMaterial->material_name = MaterialName;
	newMaterial->BaseColorID = BaseColor;
	newMaterial->NormalMapID = NormalMap;
	newMaterial->EmissiveID = Emissive;

	Materials.push_back(newMaterial);
}

void Importer_obj::CalculateTangentBitangent(
	const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3,
	const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3,
	glm::vec3& tangent, glm::vec3& bitangent)
{
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = glm::normalize(bitangent);
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
void Importer_obj::setupMesh(VertexData* VD) {
	glGenVertexArrays(1, &VD->VAO);
	glGenBuffers(1, &VD->VBO);
	glGenBuffers(1, &VD->texCoordVBO);
	glGenBuffers(1, &VD->normalVBO);
	glGenBuffers(1, &VD->tangentVBO);
	glGenBuffers(1, &VD->bitangentVBO);

	glBindVertexArray(VD->VAO);

	vector<glm::vec3> re_vertexs;
	vector<glm::vec2> re_texcoords;
	vector<glm::vec3> re_normals;
	vector<glm::vec3> tangents;
	vector<glm::vec3> bitangents;

	for (size_t i = 0; i < VD->faceIndices.size(); ++i) {
		// 각 삼각형의 정점 좌표와 텍스처 좌표 가져오기
		glm::vec3 pos1 = VD->vertexs[VD->faceIndices[i].x - 1];
		glm::vec3 pos2 = VD->vertexs[VD->faceIndices[i].y - 1];
		glm::vec3 pos3 = VD->vertexs[VD->faceIndices[i].z - 1];

		glm::vec2 uv1 = VD->texCoords[VD->texCoordIndices[i].x - 1];
		glm::vec2 uv2 = VD->texCoords[VD->texCoordIndices[i].y - 1];
		glm::vec2 uv3 = VD->texCoords[VD->texCoordIndices[i].z - 1];

		glm::vec3 normal1 = VD->normals[VD->normalIndices[i].x - 1];
		glm::vec3 normal2 = VD->normals[VD->normalIndices[i].y - 1];
		glm::vec3 normal3 = VD->normals[VD->normalIndices[i].z - 1];

		// 탱젠트 및 비트탱젠트 계산
		glm::vec3 tangent, bitangent;
		CalculateTangentBitangent(pos1, pos2, pos3, uv1, uv2, uv3, tangent, bitangent);

		re_vertexs.push_back(pos1);
		re_vertexs.push_back(pos2);
		re_vertexs.push_back(pos3);

		re_texcoords.push_back(uv1);
		re_texcoords.push_back(uv2);
		re_texcoords.push_back(uv3);

		re_normals.push_back(normal1);
		re_normals.push_back(normal2);
		re_normals.push_back(normal3);

		// 각 정점에 대해 동일한 탱젠트와 비트탱젠트를 추가
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	// VBO에 버텍스 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->VBO);
	glBufferData(GL_ARRAY_BUFFER, re_vertexs.size() * sizeof(glm::vec3), &re_vertexs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// VBO에 텍스처 좌표 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, re_texcoords.size() * sizeof(glm::vec2), &re_texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	// VBO에 노멀 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->normalVBO);
	glBufferData(GL_ARRAY_BUFFER, re_normals.size() * sizeof(glm::vec3), &re_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// VBO에 탱젠트 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->tangentVBO);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// VBO에 비트탱젠트 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VD->bitangentVBO);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
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

std::string Importer_obj::removeSubstring(const std::string& str, const std::string& toRemove) {

	std::size_t pos = str.find(toRemove);

	if (pos != std::string::npos) {
		return str.substr(pos + toRemove.length());
	}

	return str;
}



