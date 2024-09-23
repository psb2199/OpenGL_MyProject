#include "Camera.h"

Camera::Camera(Renderer* renderer, int x, int y, int z)
{
    m_Renderer = renderer;

	SetLocation(x, y, z);
}


Camera::~Camera()
{
}

void Camera::DoWorking()
{
    //ī�޶��� =============================================================
    glm::vec3 cameraPos = glm::vec3(location.x, location.y, location.z); //--- ī�޶� ��ġ
    glm::vec3 cameraDirection = glm::vec3(look_location.x, look_location.y, look_location.z); //--- ī�޶� �ٶ󺸴� ����
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    unsigned int viewLocation = glGetUniformLocation(m_Renderer->GetShader(), "view"); //--- ���� ��ȯ ����
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


    //���ٹ� ����(Perspective = ��������)
    if (isOrthoGraphic != false) {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- ���� ������ [-100.0, 100.0] �������� ����
        unsigned int projectionLocation = glGetUniformLocation(m_Renderer->GetShader(), "projection"); //--- ���� ��ȯ �� ����
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(field_of_view), 1.0f, 0.1f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- ������ �ణ �ڷ� �̷���
        unsigned int projectionLocation = glGetUniformLocation(m_Renderer->GetShader(), "projection"); //--- ���� ��ȯ �� ����
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
        //cout << "peron" << endl;
    }
}

void Camera::SetLookLocation(float x, float y, float z)
{
    look_location.x = x;
    look_location.y = y;
    look_location.z = z;
}

void Camera::SetLocation(float x, float y, float z)
{
	location.x = x;
	location.y = y;
	location.z = z;
}

vector3 Camera::GetLocation() const
{
	return location;
}
