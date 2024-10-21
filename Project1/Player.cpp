#include "Player.h"

Player::Player(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer)
	: Object(obj_id, type, loc, importer)
{
	SetMesh("Ball.obj");
	SetMaterial("Ball");
}

Player::~Player()
{
}


void Player::BeginPlayEvent()
{
	Object::BeginPlayEvent();

	
}

void Player::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	GetCamera()->SetLookLocation(GetLocation());
}
