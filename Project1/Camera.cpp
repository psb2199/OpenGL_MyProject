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
    //카메라세팅 =============================================================
    glm::vec3 cameraPos = glm::vec3(location.x, location.y, location.z); //--- 카메라 위치
    glm::vec3 cameraDirection = glm::vec3(look_location.x, look_location.y, look_location.z); //--- 카메라 바라보는 방향
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    unsigned int viewLocation = glGetUniformLocation(m_Renderer->GetShader(), "view"); //--- 뷰잉 변환 설정
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


    //원근법 유무(Perspective = 원근투영)
    if (isOrthoGraphic != false) {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정
        unsigned int projectionLocation = glGetUniformLocation(m_Renderer->GetShader(), "projection"); //--- 투영 변환 값 설정
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(field_of_view), 1.0f, 0.1f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 약간 뒤로 미뤄줌
        unsigned int projectionLocation = glGetUniformLocation(m_Renderer->GetShader(), "projection"); //--- 투영 변환 값 설정
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
