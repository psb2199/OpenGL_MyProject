#include "Object.h"

Object::Object(int obj_id, std::string type, vector3 loc, Importer_obj* importer)
{
	Importer_mesh = importer;
	id = obj_id;
	ojbect_type = type;
	//SetMesh("Male.obj");
	if(type == "Player"){ SetMesh("GravityBox.obj"); }
	else if (type == "Base") { SetMesh("Base.obj"); }
	else { SetMesh("Test.obj"); }

	location = loc;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;

}

Object::~Object()
{
}


VertexData* Object::GetMesh()
{
	return mesh;
}

void Object::SetMesh(std::string filename)
{
	mesh = Importer_mesh->FindMesh(filename);
}

void Object::SetLocation(vector3 new_location)
{
	location = new_location;
}

vector3 Object::GetLocation() const
{
	return location;
}

void Object::SetRotation(vector3 new_rotation)
{
	rotation = new_rotation;
}

vector3 Object::GetRotation() const
{
	return rotation;
}

void Object::AddMovementInput(vector3 velocity)
{
	location.x += velocity.x;
	location.y += velocity.y;
	location.z += velocity.z;
}

void Object::AddRotationInput(vector3 velocity)
{
	rotation.x += velocity.x;
	rotation.y += velocity.y;
	rotation.z += velocity.z;
}
