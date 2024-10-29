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
	setting.EnalbeGravity = false;
	setting.EnalbeCollision = true;
}

void Player::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	
	glm::vec3 camera_location = GetLocation();
	camera_location.y = 2.0;
	GetCamera()->SetLookLocation(camera_location);

	BallRolling();

	BallBouncingAnimation();
}

void Player::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);

	if (GetObjectType(collision_obj) == "Base")
	{
		hitLocation = GetLocation();

		hitVelocity = GetVelocity();
		hitVelocity.y *= -1;
		SetVelocity(hitVelocity);

		DoBounceAnim = true;
	}
	
}

void Player::BallRolling()
{
	glm::vec3 velocity = GetVelocity();
	glm::vec2 flatVelocity = { velocity.x, velocity.z };
	glm::vec3 force = GetForce();

	static glm::vec3 rotateDir;
	if (glm::length(flatVelocity) > 0.001)
	{
		rotateDir.y = atan2(velocity.x, velocity.z);
	}


	static float rotateXvalue;
	rotateXvalue -= dot(velocity, force) / 5.0;
	rotateDir.x = glm::degrees(rotateXvalue);

	SetRotation(glm::degrees(rotateDir));
}

void Player::BallBouncingAnimation()
{
	static float anim_time;
	float anim_endTime = 0.2;

	glm::vec3 currnetVel = GetVelocity();
	glm::vec3 currenLocation = GetLocation();

	if (!DoBounceAnim) anim_time = anim_endTime;
	else
	{
		anim_time -= GetDeltaTime();
		SetLocation({ currenLocation.x, hitLocation.y, currenLocation.z });
	}

	if (anim_time < 0.0)
	{
		SetVelocity({ currnetVel.x, hitVelocity.y, currnetVel.z });
		DoBounceAnim = false;
	}

	float scaleingValue = 50.0;
	glm::vec3 rescale = glm::vec3(1.0);
	rescale.y = scaleingValue * anim_time * (anim_time - anim_endTime) + 1;
	rescale.x =  -scaleingValue * anim_time * (anim_time - anim_endTime) + 1;
	rescale.z =  -scaleingValue * anim_time * (anim_time - anim_endTime) + 1;
	SetScale(rescale);
	
}
