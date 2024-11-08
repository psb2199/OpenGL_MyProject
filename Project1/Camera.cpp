#include "Camera.h"



Camera::Camera(glm::vec3 xyz)
{
	mouse_xy_value = glm::vec3{ 0.f };
	SetLocation(xyz);
}


Camera::~Camera()
{
}

glm::vec3 Camera::GetCameraDirection() const
{
	glm::vec3 direction = glm::normalize(look_location - location);
	return direction;
}

CameraMat Camera::DoWorking(GLuint Shader, float aspect)
{
	//카메라세팅 =============================================================
	location.y = sin(mouse_xy_value.y) * camera_arm_length + look_location.y;
	location.x = cos(mouse_xy_value.x) * camera_arm_length * cos(mouse_xy_value.y) + look_location.x;
	location.z = sin(mouse_xy_value.x) * camera_arm_length * cos(mouse_xy_value.y) + look_location.z;

	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(location, look_location, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(Shader, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	Camera_Matrix.view = view;

	glUniform3f(glGetUniformLocation(Shader, "u_CameraPos"), location.x, location.y, location.z);

	glm::vec3 cameraDir = glm::normalize(location - look_location);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), cameraDir)); 
	glm::vec3 up = glm::cross(cameraDir, right); 

	// 빌보드 회전 행렬 생성
	glm::mat4 billboardMatrix = glm::mat4(1.0f);
	billboardMatrix[0] = glm::vec4(right, 0.0f);
	billboardMatrix[1] = glm::vec4(up, 0.0f);
	billboardMatrix[2] = glm::vec4(cameraDir, 0.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "billboard"), 1, GL_FALSE, &billboardMatrix[0][0]);

	float yaw = atan2(cameraDir.z, cameraDir.x);
	float pitch = asin(cameraDir.y);


	//원근법 유무(Perspective = 원근투영)
	glm::mat4 projection = glm::mat4(1.0f);
	if (isOrthoGraphic) {
		projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f); //--- 투영 공간을 [-100.0, 100.0] 공간으로 설정
		unsigned int projectionLocation = glGetUniformLocation(Shader, "projection"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
	else {
		projection = glm::perspective(glm::radians(field_of_view), aspect, 0.01f, 200.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0)); //--- 공간을 약간 뒤로 미뤄줌
		unsigned int projectionLocation = glGetUniformLocation(Shader, "projection"); //--- 투영 변환 값 설정
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	}
	Camera_Matrix.projection = projection;



	return Camera_Matrix;
}

void Camera::BindWithMouseRotation(glm::vec2 xy)
{
	mouse_xy_value.x += rotate_camera_sensitive * xy.x;
	mouse_xy_value.y -= rotate_camera_sensitive * xy.y;
	if (glm::degrees(mouse_xy_value.y) > 90) mouse_xy_value.y = glm::radians(89.f);
	if (glm::degrees(mouse_xy_value.y) < -90) mouse_xy_value.y = glm::radians(-89.f);
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



