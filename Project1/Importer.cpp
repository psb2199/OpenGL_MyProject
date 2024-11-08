#include "Importer.h"

#include "Renderer.h"


Importer::Importer()
{
	Initialize();
}


Importer::~Importer()
{
}



void Importer::Initialize()
{
	// Import Obj
	{
		ReadObj("objs/Ball.obj");
		ReadObj("objs/Base.obj");
		ReadObj("objs/Coin.obj");
	}

	// Import Textures
	enum textureName {
		Grass_BaseColor,
		Grass_Normal,
		Grass_ARM,

		Ball_BaseColor,
		Ball_Normal,
		Ball_ARM,

		Coin_BaseColor,
		Coin_Normal,
		Coin_ARM,

		Base_BaseColor,
		Base_Normal,
		Base_ARM,

		Particle_Base,

		Enviroment,

		MAX
	};
	GLuint Textures[textureName(MAX)];
	{
		/*Textures[GravityBox_BaseColor] = LoadTexture("textures/GravityBox_BaseColor.png");
		Textures[GravityBox_Normal] = LoadTexture("textures/GravityBox_Normal.png");
		Textures[GravityBox_Emissive] = LoadTexture("textures/GravityBox_Emissive.png");
		Textures[GravityBox_ARM] = LoadTexture("textures/GravityBox_ARM.png");

		Textures[Grass_BaseColor] = LoadTexture("textures/Grass_BaseColor.png");
		Textures[Grass_Normal] = LoadTexture("textures/Grass_Normal.png");
		Textures[Grass_ARM] = LoadTexture("textures/Grass_ARM.png");

		Textures[Glass_BaseColor] = LoadTexture("textures/Glass/BaseColor.png");
		Textures[Glass_Normal] = LoadTexture("textures/Glass/Normal.png");
		Textures[Glass_ARM] = LoadTexture("textures/Glass/ARM.png");

		Textures[Gold_BaseColor] = LoadTexture("textures/Gold/BaseColor.png");
		Textures[Gold_Normal] = LoadTexture("textures/Gold/Normal.png");
		Textures[Gold_ARM] = LoadTexture("textures/Gold/ARM.png");*/
		Textures[Ball_BaseColor] =	LoadTexture("textures/Ball/BaseColor.png");
		Textures[Ball_Normal] =		LoadTexture("textures/Ball/Normal.png");
		Textures[Ball_ARM] =		LoadTexture("textures/Ball/ARM.png");

		Textures[Base_BaseColor] =	LoadTexture("textures/Base/BaseColor.png");
		Textures[Base_Normal] =		LoadTexture("textures/Base/Normal.png");
		Textures[Base_ARM] =		LoadTexture("textures/Base/ARM.png");

		Textures[Grass_BaseColor] = LoadTexture("textures/Grass/BaseColor.png");
		Textures[Grass_Normal] = LoadTexture("textures/Grass/Normal.png");
		Textures[Grass_ARM] = LoadTexture("textures/Grass/ARM.png");

		Textures[Coin_BaseColor] = LoadTexture("textures/Coin/BaseColor.png");
		Textures[Coin_Normal] = LoadTexture("textures/Coin/Normal.png");
		Textures[Coin_ARM] = LoadTexture("textures/Coin/ARM.png");

		Textures[Particle_Base] = LoadTexture("textures/Particle/Base.png");

		std::vector<string> cubeMap_filepathes;
		cubeMap_filepathes.push_back("textures/skybox/right.png");
		cubeMap_filepathes.push_back("textures/skybox/left.png");
		cubeMap_filepathes.push_back("textures/skybox/top.png");
		cubeMap_filepathes.push_back("textures/skybox/bottom.png");
		cubeMap_filepathes.push_back("textures/skybox/front.png");
		cubeMap_filepathes.push_back("textures/skybox/back.png");
		Textures[Enviroment] = LoadEnviromentTextures(cubeMap_filepathes);
	}

	//Set enviroment_Material
	enviroment_Material = Textures[Enviroment];

	//Make Materials
	{
		MakeMaterial(
			"Ball",
			"Basic",
			Textures[Ball_BaseColor],
			Textures[Ball_Normal],
			NULL, 
			Textures[Ball_ARM]
		);

		MakeMaterial(
			"Base",
			"Basic",
			Textures[Base_BaseColor],
			Textures[Base_Normal],
			NULL,	 
			Textures[Base_ARM]
		);

		MakeMaterial(
			"Grass",
			"Basic",
			Textures[Grass_BaseColor],
			Textures[Grass_Normal],
			NULL,
			Textures[Grass_ARM]
		);

		MakeMaterial(
			"Coin",
			"Basic",
			Textures[Coin_BaseColor],
			Textures[Coin_Normal],
			NULL,	 
			Textures[Coin_ARM]
		);

		MakeMaterial(
			"Particle",
			"Particle",
			Textures[Particle_Base],
			NULL,
			NULL,
			NULL
		);

		
	}

	//Make Particles
	VertexBuffers.push_back(ParticleMaker::CreateParticleObject("baseParticle", 100));

}


GLuint Importer::LoadTexture(const char* filepath)
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
GLuint Importer::LoadEnviromentTextures(std::vector<string> filepathes)
{
	// Load PNG
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	unsigned width, height;
	int i = 0;

	for (auto& v : filepathes) {
		std::vector<unsigned char> image;
		unsigned error = lodepng::decode(image, width, height, v.c_str());

		if (error != 0) {
			cout << "Environment image loading failed: " << v.c_str() << endl;
			continue; 
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0] 
		);

		++i;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}

Material* Importer::GetMaterial(std::string filename)
{
	for (auto& v : Materials) {

		if (v->material_name == filename) return v;
	}

	std::cout << "Can not find" << filename << "Material Asset." << std::endl;
	return nullptr;
}
GLuint Importer::GetEnviromentMaterial()
{
	return enviroment_Material;
}

void Importer::MakeMaterial(const std::string MaterialName, std::string shaderName, GLuint BaseColor, GLuint NormalMap, GLuint Emissive, GLuint ARM)
{
	Material* newMaterial = new Material;

	newMaterial->material_name = MaterialName;
	newMaterial->shader_name = shaderName;
	newMaterial->BaseColorID = BaseColor;
	newMaterial->NormalMapID = NormalMap;
	newMaterial->EmissiveID = Emissive;
	newMaterial->AoRoughnessMetallicID = ARM;

	Materials.push_back(newMaterial);
}

void Importer::CalculateTangentBitangent(
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
void Importer::ReadObj(const string filePath) {
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

			newVertexData->min_location.x = min(newVertexData->min_location.x, vertex.x);
			newVertexData->max_location.x = max(newVertexData->max_location.x, vertex.x);

			newVertexData->min_location.y = min(newVertexData->min_location.y, vertex.y);
			newVertexData->max_location.y = max(newVertexData->max_location.y, vertex.y);

			newVertexData->min_location.z = min(newVertexData->min_location.z, vertex.z);
			newVertexData->max_location.z = max(newVertexData->max_location.z, vertex.z);
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
void Importer::setupMesh(VertexData* VD) {
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


VertexData* Importer::FindMesh(std::string filename)
{
	for (std::vector<VertexData*>::iterator itr = VertexBuffers.begin(); itr != VertexBuffers.end(); ++itr)
	{
		if ((*itr)->filename == filename) return (*itr);
	}

	std::cout << "Can not find" << filename << "Mesh Asset." << std::endl;
	return nullptr;
}

void Importer::DeBugVertexData(VertexData* VD)
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

std::string Importer::removeSubstring(const std::string& str, const std::string& toRemove) {

	std::size_t pos = str.find(toRemove);

	if (pos != std::string::npos) {
		return str.substr(pos + toRemove.length());
	}

	return str;
}



