#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Importer_obj.h"

class Object
{	
	int					id;
	std::string			ojbect_type;
	VertexData*			mesh;
	Material*			m_material;

	glm::vec3			location;
	glm::vec3			rotation;
	glm::vec3			scale;

	bool				cast_shadow{ true };

	float				elapesedTime{ 0.0 };

	Importer_obj* Importer_mesh;

public:
	Object() = default;
	Object(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer);
	~Object();

	virtual void		BeginPlayEvent();
	virtual void		TickEvent(float delta_sceconds);
	float				GetElapsedTime();

	VertexData*			GetMesh();
	void				SetMesh(std::string filename);
	Material*			GetMaterial();
	void				SetMaterial(std::string filename);

	void				SetLocation(glm::vec3 new_location);
	glm::vec3			GetLocation() const;
	void				SetRotation(glm::vec3 new_rotation);
	glm::vec3			GetRotation() const;

	bool				GetCastShadow();

	void				AddMovementInput(glm::vec3 velocity);
	void				AddRotationInput(glm::vec3 velocity);
	

public:

	bool operator==(const Object& other) const {
		return this->id == other.id;
	}
};

