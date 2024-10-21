#pragma once
#include "Object.h"

class Base : public Object
{
	void BeginPlayEvent() override;
	void TickEvent(float delta_sceconds) override;

public:
	Base(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer);
	~Base();

};

