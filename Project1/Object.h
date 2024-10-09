#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Importer_obj.h"

class Object
{	
	int id;
	std::string ojbect_type;
	VertexData* mesh;
	Material* m_material;

	glm::vec3 location;
	glm::vec3 rotation;

	Importer_obj* Importer_mesh;
public:
	Object(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer);
	~Object();

	VertexData* GetMesh();
	void SetMesh(std::string filename);
	Material* GetMaterial();
	void SetMaterial(std::string filename);

	void SetLocation(glm::vec3 new_location);
	glm::vec3 GetLocation() const;
	void SetRotation(glm::vec3 new_rotation);
	glm::vec3 GetRotation() const;

	void AddMovementInput(glm::vec3 velocity);
	void AddRotationInput(glm::vec3 velocity);
	

public:

	bool operator==(const Object& other) const {
		return this->id == other.id;
	}
};

