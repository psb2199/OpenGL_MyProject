#include "Player.h"

Player::Player(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, std::vector<Object*>* _AllObjects)
	: Object(obj_id, type, loc, importer, _AllObjects)
{
	SetMesh("Ball.obj");
	SetMaterial("Ball");

	BeginPlayEvent();
}

Player::~Player()
{
}


void Player::BeginPlayEvent()
{
	Object::BeginPlayEvent();

	setting.isStatic = false;
	setting.EnalbeGravity = true;
	setting.EnalbeCollision = true;
}

void Player::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	
	glm::vec3 camera_location = GetLocation();
	camera_location.y = 2.0;
	GetCamera()->SetLookLocation(camera_location);

	glm::vec3 velocity = GetVelocity();
	glm::vec3 force = GetForce();
	glm::vec3 rotateDir = glm::vec3{ 0.0 };
	
	rotateDir.y = atan2(velocity.x, velocity.z);
	static float rotateXvalue;
	rotateXvalue -= dot(velocity, force) / 5.0;
	rotateDir.x = glm::degrees(rotateXvalue);
	
	SetRotation(glm::degrees(rotateDir));
}

void Player::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);

	if (GetObjectType(collision_obj) == "Base")
	{
		glm::vec3 newVel = GetVelocity();
		newVel.y *= -1.0;
		SetVelocity(newVel);
	}
	
}
