#pragma once
#include "OpenGL.h"

#include "Object.h"

class Coin : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;

public:
	Coin(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, ObjectManager* objmgr);
	~Coin();

};

