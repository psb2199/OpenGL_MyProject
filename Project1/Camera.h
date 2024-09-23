#pragma once
#include "OpenGL.h"
#include "Renderer.h"

struct vector3 { float x, y, z; };

class Camera
{
	Renderer* m_Renderer;


	vector3 look_location;
	vector3 location;

	float field_of_view{ 90 };

	bool isOrthoGraphic{ false };

public:
	Camera(Renderer* renderer, int x, int y, int z);
	~Camera();

	void DoWorking();

	void SetLookLocation(float x, float y, float z);
	void SetLocation(float x, float y, float z);
	vector3 GetLocation() const;
};

