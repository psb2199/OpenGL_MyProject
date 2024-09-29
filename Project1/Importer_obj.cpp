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
	ReadObj("Test.obj");
	ReadObj("GravityBox.obj");
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
		
	}
	std::cout << std::endl;

	for (auto& v : VD->texCoords) {
		//std::cout << v.x << " " << v.y << std::endl;
		count++;
	}

	for (auto& v : VD->normalIndices) {
		//std::cout << v.x << " " << v.y << " " << v.z << std::endl;
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
	newVertexData->filename = filePath;

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
		}
	}


	DeBugVertexData(newVertexData);
	// 메쉬 설정 (버텍스, 텍스처, 노멀 좌표 및 인덱스 데이터로)
	setupMesh(newVertexData);

	// VertexBuffer 리스트에 추가
	VertexBuffers.push_back(newVertexData);
}


GLuint Importer_obj::FindMesh(std::string filename)
{
	for (std::vector<VertexData*>::iterator itr = VertexBuffers.begin(); itr != VertexBuffers.end(); ++itr)
	{
		if ((*itr)->filename == filename) return(*itr)->VAO;
	}

	std::cout << "Can not find" << filename << "Mesh Asset." << std::endl;
	return -1;
}


void Importer_obj::setupMesh(VertexData* VB) {
	// VAO, VBO, EBO 생성
	glGenVertexArrays(1, &VB->VAO);
	glGenBuffers(1, &VB->VBO);
	glGenBuffers(1, &VB->texCoordVBO);
	glGenBuffers(1, &VB->normalVBO);
	glGenBuffers(1, &VB->EBO);


	// VAO 바인딩
	glBindVertexArray(VB->VAO);

	// VBO에 버텍스 데이터 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VB->VBO);
	glBufferData(GL_ARRAY_BUFFER, VB->vertexs.size() * sizeof(glm::vec3), &VB->vertexs[0], GL_STATIC_DRAW);

	// 버텍스 속성 포인터 설정
	glEnableVertexAttribArray(0); // 위치 속성 인덱스
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	//텍스처 좌표가 있는 경우 VBO에 텍스처 좌표 데이터 바인딩
	if (!VB->texCoords.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, VB->texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, VB->texCoords.size() * sizeof(glm::vec2), &VB->texCoords[0], GL_STATIC_DRAW);

		// 텍스처 좌표 속성 포인터 설정
		glEnableVertexAttribArray(1); // 텍스처 좌표 속성 인덱스
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // 텍스처 좌표 VBO 해제
	}


	// 노말이 있는 경우 VBO에 노말 데이터 바인딩
	if (!VB->normalIndices.empty()) {
		// 각 버텍스에 대응하는 노말 데이터를 저장할 벡터를 만듭니다.
		std::vector<glm::vec3> expandedNormals(VB->vertexs.size());

		// 노말 인덱스를 따라 노말 값을 버텍스에 할당합니다.
		for (size_t i = 0; i < VB->faceIndices.size(); ++i) {
			glm::vec3 vertexIndex = VB->faceIndices[i];
			glm::vec3 normalIndex = VB->normalIndices[i];

			// 각 버텍스 인덱스에 해당하는 노말을 할당합니다.
			expandedNormals[vertexIndex.x - 1] = VB->normals[normalIndex.x - 1];
			expandedNormals[vertexIndex.y - 1] = VB->normals[normalIndex.y - 1];
			expandedNormals[vertexIndex.z - 1] = VB->normals[normalIndex.z - 1];
		}

		// VBO 생성 및 바인딩
		GLuint normalVBO;
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, expandedNormals.size() * sizeof(glm::vec3), &expandedNormals[0], GL_STATIC_DRAW);

		// 노말 속성 포인터 설정
		glEnableVertexAttribArray(2); // 노말 속성 인덱스
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // 노말 VBO 해제
	}


	// EBO에 인덱스 데이터 바인딩
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VB->EBO);
	std::vector<GLuint> indices; // 인덱스 버퍼에 사용할 인덱스 데이터 준비

	// faceIndices를 통해 인덱스 데이터를 만듭니다.
	for (size_t i = 0; i < VB->faceIndices.size(); ++i) {
		indices.push_back(static_cast<GLuint>(VB->faceIndices[i].x) - 1);
		indices.push_back(static_cast<GLuint>(VB->faceIndices[i].y) - 1);
		indices.push_back(static_cast<GLuint>(VB->faceIndices[i].z) - 1);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);



	// VAO 해제
	glBindVertexArray(0);
}





