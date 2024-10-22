#include "Camera.h"



Camera::Camera(glm::vec3 xyz)
{
    rotation = glm::vec3{ 0.f };
	SetLocation(xyz);
}


Camera::~Camera()
{
}

CameraMat Camera::DoWorking(GLuint Shader, float aspect)
{
    //카메라세팅 =============================================================
   location.y = sin(rotation.y) * camera_arm_length + look_location.y;
   location.x = cos(rotation.x) * camera_arm_length * cos(rotation.y) + look_location.x;
   location.z = sin(rotation.x) * camera_arm_length * cos(rotation.y) + look_location.z;


    glm::vec3 cameraPos = location; //--- 카메라 위치
    glm::vec3 cameraDirection = look_location; //--- 카메라 바라보는 방향
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    unsigned int viewLocation = glGetUniformLocation(Shader, "view"); //--- 뷰잉 변환 설정
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    Camera_Matrix.view = view;

    glUniform3f(glGetUniformLocation(Shader, "u_CameraPos"), location.x, location.y, location.z);

    //원근법 유무(Perspective = 원근투영)
    glm::mat4 projection = glm::mat4(1.0f);
    if (isOrthoGraphic) {
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정
        unsigned int projectionLocation = glGetUniformLocation(Shader, "projection"); //--- 투영 변환 값 설정
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {
        projection = glm::perspective(glm::radians(field_of_view), aspect, 0.01f, 100.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 약간 뒤로 미뤄줌
        unsigned int projectionLocation = glGetUniformLocation(Shader, "projection"); //--- 투영 변환 값 설정
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    Camera_Matrix.projection = projection;

  

    return Camera_Matrix;
}

void Camera::BindWithMouseRotation(glm::vec2 xy)
{
    rotation.x += rotate_camera_sensitive * xy.x;
    rotation.y -= rotate_camera_sensitive * xy.y;
    if (RadianToDegree(rotation.y) > 90) rotation.y = DegreeToRadian(89.f);
    if (RadianToDegree(rotation.y) < -90) rotation.y = DegreeToRadian(-89.f);
}

void Camera::BindWithMouseWheel(float value)
{
    float value_size{ 0.1 };
    camera_arm_length += value * value_size;
    if (camera_arm_length < 0.1) camera_arm_length = 0.1;
}

void Camera::SetLookLocation(glm::vec3 loc)
{
    look_location = loc;
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

std::array<Plane, 6> Camera::extractFrustumPlanes(const glm::mat4& projViewMatrix)
{
    std::array<Plane, 6> planes;

    planes[0].normal = glm::vec3(projViewMatrix[0][3] + projViewMatrix[0][0], // Left
        projViewMatrix[1][3] + projViewMatrix[1][0],
        projViewMatrix[2][3] + projViewMatrix[2][0]);
    planes[0].distance = projViewMatrix[3][3] + projViewMatrix[3][0];

    planes[1].normal = glm::vec3(projViewMatrix[0][3] - projViewMatrix[0][0], // Right
        projViewMatrix[1][3] - projViewMatrix[1][0],
        projViewMatrix[2][3] - projViewMatrix[2][0]);
    planes[1].distance = projViewMatrix[3][3] - projViewMatrix[3][0];

    planes[2].normal = glm::vec3(projViewMatrix[0][3] + projViewMatrix[0][1], // Bottom
        projViewMatrix[1][3] + projViewMatrix[1][1],
        projViewMatrix[2][3] + projViewMatrix[2][1]);
    planes[2].distance = projViewMatrix[3][3] + projViewMatrix[3][1];

    planes[3].normal = glm::vec3(projViewMatrix[0][3] - projViewMatrix[0][1], // Top
        projViewMatrix[1][3] - projViewMatrix[1][1],
        projViewMatrix[2][3] - projViewMatrix[2][1]);
    planes[3].distance = projViewMatrix[3][3] - projViewMatrix[3][1];

    planes[4].normal = glm::vec3(projViewMatrix[0][3] + projViewMatrix[0][2], // Near
        projViewMatrix[1][3] + projViewMatrix[1][2],
        projViewMatrix[2][3] + projViewMatrix[2][2]);
    planes[4].distance = projViewMatrix[3][3] + projViewMatrix[3][2];

    planes[5].normal = glm::vec3(projViewMatrix[0][3] - projViewMatrix[0][2], // Far
        projViewMatrix[1][3] - projViewMatrix[1][2],
        projViewMatrix[2][3] - projViewMatrix[2][2]);
    planes[5].distance = projViewMatrix[3][3] - projViewMatrix[3][2];

    return planes;
}

bool Camera::isCollisionBoxInFrustum(const std::array<Plane, 6>& planes, const CollisionBox& box)
{
    glm::vec3 corners[8] = {
        glm::vec3(box.min.x, box.min.y, box.min.z),
        glm::vec3(box.max.x, box.min.y, box.min.z),
        glm::vec3(box.min.x, box.max.y, box.min.z),
        glm::vec3(box.max.x, box.max.y, box.min.z),
        glm::vec3(box.min.x, box.min.y, box.max.z),
        glm::vec3(box.max.x, box.min.y, box.max.z),
        glm::vec3(box.min.x, box.max.y, box.max.z),
        glm::vec3(box.max.x, box.max.y, box.max.z),
    };

   
    for (const auto& plane : planes) {
        bool allPointsOutside = true;

       
        for (const auto& corner : corners) {
            if (glm::dot(plane.normal, corner) + plane.distance > 0) {
                allPointsOutside = false;
                break; 
            }
        }

        if (allPointsOutside) {
            return false;
        }
    }

    return true;
}



