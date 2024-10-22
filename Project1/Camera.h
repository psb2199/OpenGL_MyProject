#pragma once
#include "OpenGL.h"

#define M_PI 3.14159265358979323846

struct CameraMat {
	glm::mat4 view = glm::mat4(1.0);
	glm::mat4 projection = glm::mat4(1.0);
};

struct Plane {
	glm::vec3 normal;
	float distance;
};

class Camera
{
	glm::vec3 look_location;
	glm::vec3 location;
	CameraMat Camera_Matrix;

	glm::vec3 rotation;

	float camera_arm_length{ 20.f };

	float rotate_camera_sensitive{ 0.001 };
	float field_of_view{ 45 };
	bool isOrthoGraphic{ false };

	float RadianToDegree(float value);
	float DegreeToRadian(float value);

public:
	Camera(glm::vec3 xyz);
	~Camera();

	CameraMat DoWorking(GLuint Shader, float aspect);
	bool isCollisionBoxInFrustum(const std::array<Plane, 6>& planes, const CollisionBox& box);
	std::array<Plane, 6> extractFrustumPlanes(const glm::mat4& projViewMatrix);

	void BindWithMouseRotation(glm::vec2 xy);
	void BindWithMouseWheel(float value);

	void SetLookLocation(glm::vec3 loc);
	void SetLocation(glm::vec3 xyz);
	glm::vec3 GetLocation() const;
};

