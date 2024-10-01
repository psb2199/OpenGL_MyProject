#pragma once
#include "OpenGL.h"
#include "Renderer.h"

#define M_PI 3.14159265358979323846

class Camera
{
	Renderer* m_Renderer;


	glm::vec3 look_location;
	glm::vec3 location;
	glm::vec3 rotation;

	float camera_arm_length{ 3.f };

	float rotate_camera_sensitive{ 0.001 };
	float field_of_view{ 90 };
	bool isOrthoGraphic{ false };

	float RadianToDegree(float value);
	float DegreeToRadian(float value);

public:
	Camera(Renderer* renderer, glm::vec3 xyz);
	~Camera();

	void DoWorking(Renderer* renderer);

	void BindWithMouse(glm::vec2 xy);

	void SetLookLocation(float x, float y, float z);
	void SetLocation(glm::vec3 xyz);
	glm::vec3 GetLocation() const;
};

