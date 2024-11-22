#pragma once
#include "OpenGL.h"

#include "Object.h"

class Base : public Object
{
	void BeginPlayEvent() override;

public:
	Base(int obj_id, int type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	~Base();

};

