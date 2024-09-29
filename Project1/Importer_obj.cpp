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
}

void Importer_obj::ReadObj(const string filePath) {
    ifstream objFile(filePath);

    if (!objFile.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        return;
    }
    else { cout << "File Load Success:" << filePath << endl; }

    VertexBuffer* newVertexBuffer = new VertexBuffer;
    newVertexBuffer->filename = filePath;

    vector<glm::vec3> vertices;
    vector<unsigned int> indices;

    string line;
    vector<glm::vec3> tempVertices;
    vector<vector<unsigned int>> tempFaces;

    // 1. 파일에서 버텍스 및 인덱스 데이터 읽기
    while (getline(objFile, line)) {
        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (type == "f") {
            unsigned int v1, v2, v3;
            char dummy; // '/'를 무시하기 위한 더미 변수
            ss >> v1 >> dummy >> dummy >> v2 >> dummy >> dummy >> v3;
            indices.push_back(v1 - 1); // OBJ 파일의 인덱스는 1부터 시작하므로 0부터 시작하도록 조정
            indices.push_back(v2 - 1);
            indices.push_back(v3 - 1);
        }
    }
    vertices = tempVertices;

    setupMesh(newVertexBuffer, vertices, indices);

    VertexBuffers.push_back(newVertexBuffer);
}

GLuint Importer_obj::FindMesh(std::string filename)
{
    for (std::vector<VertexBuffer*>::iterator itr = VertexBuffers.begin(); itr != VertexBuffers.end(); ++itr)
    {
        if ((*itr)->filename == filename) return(*itr)->VAO;
    }

    std::cout << "Can not find" << filename << "Mesh Asset." << std::endl;
    return -1;
}


// OpenGL을 사용하여 버텍스 및 인덱스 데이터를 VBO, EBO에 넣기
void Importer_obj::setupMesh(VertexBuffer* VB, const vector<glm::vec3>& vertices, const vector<unsigned int>& indices) {
    
    glGenVertexArrays(1, &VB->VAO);
    glGenBuffers(1, &VB->VBO);
    glGenBuffers(1, &VB->EBO);

    glBindVertexArray(VB->VAO);

    // VBO에 버텍스 데이터 전송
    glBindBuffer(GL_ARRAY_BUFFER, VB->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // EBO에 인덱스 데이터 전송
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VB->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 버텍스 속성 지정 (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // VAO 언바인드
}

// OpenGL에서 그리기
//void render() {
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 인덱스 기반 그리기
//    glBindVertexArray(0);
//}



