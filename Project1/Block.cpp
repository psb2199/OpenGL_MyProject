#include "Block.h"

Block::Block(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer)
	: Object(obj_id, type, loc, importer)
{
}

Block::~Block()
{
}

void Block::BeginPlayEvent()
{
	Object::BeginPlayEvent();


}

void Block::TickEvent(float delta_sceconds)
{
	Object::TickEvent(delta_sceconds);


}