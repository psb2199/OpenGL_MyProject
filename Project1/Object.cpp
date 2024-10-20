#include "Object.h"

Object::Object(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer)
{
	Importer_mesh = importer;
	id = obj_id;
	ojbect_type = type;
	
	if (type == "Player") {
		//SetMesh("Sphere.obj");
		//SetMaterial("M_Glass");
		//SetMaterial("M_Gold");
		//SetMesh("Male.obj");
		//SetMaterial("M_Male");
		SetMesh("GravityBox.obj");
		SetMaterial("M_GravityBox");
	}
	else if (type == "Base"){
		SetMesh("Base.obj");
		SetMaterial("M_Grass");
	}
	else if (type == "Test"){
		SetMesh("Sphere.obj");
		SetMaterial("M_Glass");
	}


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

Material* Object::GetMaterial()
{
	return m_material;
}

void Object::SetMaterial(std::string filename)
{
	m_material = Importer_mesh->GetMaterial(filename);
}



void Object::SetLocation(glm::vec3 new_location)
{
	location = new_location;
}

glm::vec3 Object::GetLocation() const
{
	return location;
}

void Object::SetRotation(glm::vec3 new_rotation)
{
	rotation = new_rotation;
}

glm::vec3 Object::GetRotation() const
{
	return rotation;
}

bool Object::GetCastShadow()
{
	return cast_shadow;
}

void Object::AddMovementInput(glm::vec3 velocity)
{
	location.x += velocity.x;
	location.y += velocity.y;
	location.z += velocity.z;
}

void Object::AddRotationInput(glm::vec3 velocity)
{
	rotation.x += velocity.x;
	rotation.y += velocity.y;
	rotation.z += velocity.z;
}
