#pragma once
#include "Object.h"

class Player : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;
	void OverlapedCollisionEvent(Object* collision_obj) override;

public:
	Player(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, std::vector<Object*>* _AllObjects);
	~Player();
};

