#include "Controller.h"

Controller::Controller()
{
	for (int i{ 0 }; i < press(KEY_COUNT); ++i) { Key[i] = false; }
}

Controller::~Controller()
{
}

void Controller::MappingController(Object* obj)
{
	mapped_obj = obj;
}

void Controller::TickEvent(float delta_seconds)
{
	if (!mapped_obj) return;

	controller_direction = mapped_obj->GetCamera()->GetCameraDirection();

	// Ignore Pitch Value
	glm::vec3 forwardDirection = glm::normalize(glm::vec3(controller_direction.x, 0, controller_direction.z));

	// Calculate the right direction 
	glm::vec3 rightDirection = glm::normalize(glm::cross(forwardDirection, glm::vec3(0, 1, 0)));

	float movespeed = 0.025;

	if (Key[press(w)])
	{
		//mapped_obj->AddRotationInput({ 0,0,1 });
		mapped_obj->SetForce(forwardDirection * movespeed);
	}
	if (Key[press(s)])
	{
		//mapped_obj->AddRotationInput({ 0,0,-1 });
		mapped_obj->SetForce(-forwardDirection * movespeed);
	}
	if (Key[press(a)])
	{
		//mapped_obj->AddRotationInput({ 0,1,0 });
		mapped_obj->SetForce(-rightDirection * movespeed);
	}
	if (Key[press(d)])
	{
		//mapped_obj->AddRotationInput({ 0,-1,0 });
		mapped_obj->SetForce(rightDirection * movespeed);
	}
}

void Controller::Debug_print()
{
	for (int i{ 0 }; i < press(KEY_COUNT); ++i)
	{
		std::cout << Key[i];
	}
	std::cout << std::endl;
}


