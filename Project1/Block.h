#pragma once
#include "Object.h"

class Block : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;

public:
	Block(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer);
	~Block();

};

