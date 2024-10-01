#include "Camera.h"



Camera::Camera(Renderer* renderer, glm::vec3 xyz)
{
    m_Renderer = renderer;

	SetLocation(xyz);
}


Camera::~Camera()
{
}

void Camera::DoWorking(Renderer* renderer)
{
    //ī�޶��� =============================================================
    location.y = sin(rotation.y) * camera_arm_length;
    location.x = cos(rotation.x) * camera_arm_length * cos(rotation.y);
    location.z = sin(rotation.x) * camera_arm_length * cos(rotation.y);


    glm::vec3 cameraPos = location; //--- ī�޶� ��ġ
    glm::vec3 cameraDirection = look_location; //--- ī�޶� �ٶ󺸴� ����
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    unsigned int viewLocation = glGetUniformLocation(renderer->GetShader(), "view"); //--- ���� ��ȯ ����
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


    //���ٹ� ����(Perspective = ��������)
    if (isOrthoGraphic) {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- ���� ������ [-100.0, 100.0] �������� ����
        unsigned int projectionLocation = glGetUniformLocation(renderer->GetShader(), "projection"); //--- ���� ��ȯ �� ����
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(field_of_view), 1.0f, 0.01f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- ������ �ణ �ڷ� �̷���
        unsigned int projectionLocation = glGetUniformLocation(renderer->GetShader(), "projection"); //--- ���� ��ȯ �� ����
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
}

void Camera::BindWithMouse(glm::vec2 xy)
{
    rotation.x += rotate_camera_sensitive * xy.x;
    rotation.y -= rotate_camera_sensitive * xy.y;
    if (RadianToDegree(rotation.y) > 90)  rotation.y = DegreeToRadian(89.f);
    if (RadianToDegree(rotation.y) < -90)  rotation.y = DegreeToRadian(-89.f);
}

void Camera::SetLookLocation(float x, float y, float z)
{
    look_location.x = x;
    look_location.y = y;
    look_location.z = z;
}

void Camera::SetLocation(glm::vec3 xyz)
{
	location = xyz;
}

glm::vec3 Camera::GetLocation() const
{
	return location;
}

float Camera::RadianToDegree(float value)
{
    return value * 180.0 / M_PI;
}

float Camera::DegreeToRadian(float value)
{
    return value * M_PI / 180.0;
}
