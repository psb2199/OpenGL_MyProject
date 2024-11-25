#include "Player.h"

#include "ObjectManager.h"

Player::Player(int obj_id, int type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr)
	: Object(obj_id, type, loc, importer, objmgr)
{
	

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
	setting.EnalbeCollision = false;
	setting.EnableRendering = false;
}

void Player::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);

	
	glm::vec3 camera_location = GetLocation();
	GetCamera()->SetLookLocation(camera_location);


}



void Player::OverlapedCollisionEvent(Object* collision_obj)
{
	Object::OverlapedCollisionEvent(collision_obj);

	
}




