#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Importer_obj.h"
#include "Camera.h"

#define GRAVITY	0.98;

struct Setting {
	bool				isStatic{ false };
	bool				EnalbeGravity{ false };
	bool				cast_shadow{ true };
	bool				EnalbeCollision{ false };
};

struct CollisionBox {
	glm::vec3 min;
	glm::vec3 max;
};

class Object
{	
	int					id;
	std::string			ojbect_type;
	VertexData*			mesh;
	CollisionBox		collision_range;
	Material*			m_material;

	glm::vec3			location;
	glm::vec3			rotation;
	glm::vec3			scale;

	glm::vec3			velocity;


	float				elapesedTime{ 0.0 };

	Camera*				m_Camera;
	Importer_obj*		Importer_mesh;
	std::vector<Object*>* AllObjects;
	
	void				SetCollisionRange();
	bool				CheckCollision(const CollisionBox& box1, const CollisionBox& box2);
	void				CheckAllCollisions(std::vector<Object*>& WorldObjects);
	CollisionBox		GetCollisionRange() const;
public:
	Setting				setting;

	Object() = default;
	Object(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, std::vector<Object*>* _AllObjects);
	~Object();

	virtual void		BeginPlayEvent();
	virtual void		TickEvent(float delta_sceconds);
	virtual void		OverlapedCollisionEvent(Object* collision_obj);
	float				GetElapsedTime();

	void				SetCamera(Camera* camera);
	Camera*				GetCamera();

	int					GetID();

	VertexData*			GetMesh();
	void				SetMesh(std::string filename);
	Material*			GetMaterial();
	void				SetMaterial(std::string filename);

	void				SetLocation(glm::vec3 new_location);
	glm::vec3			GetLocation() const;
	void				SetRotation(glm::vec3 new_rotation);
	glm::vec3			GetRotation() const;

	void				AddMovementInput(glm::vec3 velocity);
	void				AddRotationInput(glm::vec3 velocity);
	glm::vec3			GetVelocity() const;
	

public:

	bool operator==(const Object& other) const {
		return this->id == other.id;
	}
};

