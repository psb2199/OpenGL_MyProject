#pragma once
#include "OpenGL.h"


#define M_PI 3.14159265358979323846

class Camera
{

	glm::vec3 look_location;
	glm::vec3 location;
	glm::vec3 rotation;

	float camera_arm_length{ 3.f };

	float rotate_camera_sensitive{ 0.001 };
	float field_of_view{ 45 };
	bool isOrthoGraphic{ false };

	float RadianToDegree(float value);
	float DegreeToRadian(float value);

public:
	Camera(glm::vec3 xyz);
	~Camera();

	void DoWorking(GLuint Shader, float aspect);

	void BindWithMouseRotation(glm::vec2 xy);
	void BindWithMouseWheel(float value);

	void SetLookLocation(float x, float y, float z);
	void SetLocation(glm::vec3 xyz);
	glm::vec3 GetLocation() const;
};

