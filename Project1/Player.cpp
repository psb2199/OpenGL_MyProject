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

}

void Player::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	

	GetCamera()->SetLookLocation(GetLocation());
}

void Player::OverlapedCollisionEvent(Object* collision_obj)
{
	cout << collision_obj->GetID() << endl;
}
