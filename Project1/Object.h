#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Importer_obj.h"

class Object
{	
	int id;
	std::string ojbect_type;
	GLuint mesh;

	vector3 location;
	vector3 rotation;

	Importer_obj* Importer_mesh;
public:
	Object(int obj_id, std::string type, vector3 loc, Importer_obj* importer);
	~Object();

	GLuint GetMesh() const;
	void SetMesh(std::string filename);

	void SetLocation(vector3 new_location);
	vector3 GetLocation() const;
	void SetRotation(vector3 new_rotation);
	vector3 GetRotation() const;

	void AddMovementInput(vector3 velocity);
	void AddRotationInput(vector3 velocity);
	

public:

	bool operator==(const Object& other) const {
		return this->id == other.id;
	}
};

