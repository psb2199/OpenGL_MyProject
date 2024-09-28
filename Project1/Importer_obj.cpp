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
    ReadObj("Test.obj", VertexBuffers[ObjFile(test)]);
}

void Importer_obj::ReadObj(const string filePath, VertexBuffer VB) {
    ifstream objFile(filePath);

    if (!objFile.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        return;
    }
    else { cout << "File Load Success:" << filePath << endl; }

    VB.filename = filePath;

    vector<glm::vec3> vertices;
    vector<unsigned int> indices;

    string line;
    vector<glm::vec3> tempVertices;
    vector<vector<unsigned int>> tempFaces;

    // 1. ���Ͽ��� ���ؽ� �� �ε��� ������ �б�
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
            char dummy; // '/'�� �����ϱ� ���� ���� ����
            ss >> v1 >> dummy >> dummy >> v2 >> dummy >> dummy >> v3;
            indices.push_back(v1 - 1); // OBJ ������ �ε����� 1���� �����ϹǷ� 0���� �����ϵ��� ����
            indices.push_back(v2 - 1);
            indices.push_back(v3 - 1);
        }
    }
    vertices = tempVertices;

    setupMesh(VB, vertices, indices);
}


// OpenGL�� ����Ͽ� ���ؽ� �� �ε��� �����͸� VBO, EBO�� �ֱ�
void Importer_obj::setupMesh(VertexBuffer VB, const vector<glm::vec3>& vertices, const vector<unsigned int>& indices) {
    
    glGenVertexArrays(1, &VB.VAO);
    glGenBuffers(1, &VB.VBO);
    glGenBuffers(1, &VB.EBO);

    glBindVertexArray(VB.VAO);

    // VBO�� ���ؽ� ������ ����
    glBindBuffer(GL_ARRAY_BUFFER, VB.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // EBO�� �ε��� ������ ����
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VB.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // ���ؽ� �Ӽ� ���� (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // VAO ����ε�
}

// OpenGL���� �׸���
//void render() {
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // �ε��� ��� �׸���
//    glBindVertexArray(0);
//}



