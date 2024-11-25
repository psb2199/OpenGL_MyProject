#pragma once
#include "OpenGL.h"

#include "Object.h"

class ObjectManager;

class Player : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;
	void OverlapedCollisionEvent(Object* collision_obj) override;


public:
	Player(int obj_id,int type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	~Player();
};

