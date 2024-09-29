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

	// 1. ���Ͽ��� ���ؽ�, �ؽ�ó ��ǥ, ��� �� �ε��� ������ �б�
	while (getline(objFile, line)) {
		if (line.empty()) continue;  // �� �� ����

		stringstream ss(line);
		string type;
		ss >> type;

		if (type == "v") {
			// ���ؽ� ��ǥ �б�
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			newVertexData->vertexs.push_back(vertex);

		}
		else if (type == "vt") {
			// �ؽ�ó ��ǥ �б�
			glm::vec2 texCoord;
			ss >> texCoord.x >> texCoord.y;
			newVertexData->texCoords.push_back(texCoord);

		}
		else if (type == "vn") {
			// ��� ��ǥ �б�
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
	// �޽� ���� (���ؽ�, �ؽ�ó, ��� ��ǥ �� �ε��� �����ͷ�)
	setupMesh(newVertexData);

	// VertexBuffer ����Ʈ�� �߰�
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
	// VAO, VBO, EBO ����
	glGenVertexArrays(1, &VB->VAO);
	glGenBuffers(1, &VB->VBO);
	glGenBuffers(1, &VB->texCoordVBO);
	glGenBuffers(1, &VB->normalVBO);
	glGenBuffers(1, &VB->EBO);


	// VAO ���ε�
	glBindVertexArray(VB->VAO);

	// VBO�� ���ؽ� ������ ���ε�
	glBindBuffer(GL_ARRAY_BUFFER, VB->VBO);
	glBufferData(GL_ARRAY_BUFFER, VB->vertexs.size() * sizeof(glm::vec3), &VB->vertexs[0], GL_STATIC_DRAW);

	// ���ؽ� �Ӽ� ������ ����
	glEnableVertexAttribArray(0); // ��ġ �Ӽ� �ε���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	//�ؽ�ó ��ǥ�� �ִ� ��� VBO�� �ؽ�ó ��ǥ ������ ���ε�
	if (!VB->texCoords.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, VB->texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, VB->texCoords.size() * sizeof(glm::vec2), &VB->texCoords[0], GL_STATIC_DRAW);

		// �ؽ�ó ��ǥ �Ӽ� ������ ����
		glEnableVertexAttribArray(1); // �ؽ�ó ��ǥ �Ӽ� �ε���
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // �ؽ�ó ��ǥ VBO ����
	}


	// �븻�� �ִ� ��� VBO�� �븻 ������ ���ε�
	if (!VB->normalIndices.empty()) {
		// �� ���ؽ��� �����ϴ� �븻 �����͸� ������ ���͸� ����ϴ�.
		std::vector<glm::vec3> expandedNormals(VB->vertexs.size());

		// �븻 �ε����� ���� �븻 ���� ���ؽ��� �Ҵ��մϴ�.
		for (size_t i = 0; i < VB->faceIndices.size(); ++i) {
			glm::vec3 vertexIndex = VB->faceIndices[i];
			glm::vec3 normalIndex = VB->normalIndices[i];

			// �� ���ؽ� �ε����� �ش��ϴ� �븻�� �Ҵ��մϴ�.
			expandedNormals[vertexIndex.x - 1] = VB->normals[normalIndex.x - 1];
			expandedNormals[vertexIndex.y - 1] = VB->normals[normalIndex.y - 1];
			expandedNormals[vertexIndex.z - 1] = VB->normals[normalIndex.z - 1];
		}

		// VBO ���� �� ���ε�
		GLuint normalVBO;
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, expandedNormals.size() * sizeof(glm::vec3), &expandedNormals[0], GL_STATIC_DRAW);

		// �븻 �Ӽ� ������ ����
		glEnableVertexAttribArray(2); // �븻 �Ӽ� �ε���
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // �븻 VBO ����
	}


	// EBO�� �ε��� ������ ���ε�
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VB->EBO);
	std::vector<GLuint> indices; // �ε��� ���ۿ� ����� �ε��� ������ �غ�

	// faceIndices�� ���� �ε��� �����͸� ����ϴ�.
	for (size_t i = 0; i < VB->faceIndices.size(); ++i) {
		indices.push_back(static_cast<GLuint>(VB->faceIndices[i].x) - 1);
		indices.push_back(static_cast<GLuint>(VB->faceIndices[i].y) - 1);
		indices.push_back(static_cast<GLuint>(VB->faceIndices[i].z) - 1);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);



	// VAO ����
	glBindVertexArray(0);
}





