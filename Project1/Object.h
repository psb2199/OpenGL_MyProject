#pragma once
#include <iostream>

#include "OpenGL.h"
#include "Importer_obj.h"
#include "Camera.h"

#define GRAVITY	1

struct Setting {
	bool				isStatic{ false };
	bool				EnalbeGravity{ false };
	bool				cast_shadow{ true };
	bool				EnalbeCollision{ false };
	bool				EnableRendering{ true };
};


class Object
{	
	int					id;
	std::string			ojbect_type;
	VertexData*			mesh;
	CollisionBox		origin_collision_range;
	CollisionBox		collision_range;
	Material*			m_material;

	glm::vec3			location;
	glm::vec3			rotation;
	glm::vec3			scale;
	float				mass;
	glm::vec3			velocity;
	glm::vec3			force;
	float				friction{ 0.1 };


	bool				isOverlapped{ false };

	float				elapesedTime{ 0.0 };

	Camera*				m_Camera;
	Importer_obj*		Importer_mesh;
	std::vector<Object*>* AllObjects;

	
	void				SetCollisionRange();
	void				UpdateCollisionRange();
	bool				CheckCollision(const CollisionBox& box1, const CollisionBox& box2);
	void				CheckAllCollisions(std::vector<Object*>& WorldObjects);
public:
	Setting				setting;

	Object() = default;
	Object(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, std::vector<Object*>* _AllObjects);
	~Object();

	virtual void		BeginPlayEvent();
	virtual void		TickEvent(float delta_seconds);
	virtual void		OverlapedCollisionEvent(Object* collision_obj);
	float				GetElapsedTime();

	CollisionBox		GetCollisionRange() const;
	void				SetCamera(Camera* camera);
	Camera*				GetCamera();

	int					GetID();
	std::string			GetObjectType(Object* obj);

	VertexData*			GetMesh();
	void				SetMesh(std::string filename);
	Material*			GetMaterial();
	void				SetMaterial(std::string filename);

	void				SetLocation(glm::vec3 new_location);
	glm::vec3			GetLocation() const;
	void				SetRotation(glm::vec3 new_rotation);
	glm::vec3			GetRotation() const;
	void				SetScale(glm::vec3 new_scale);
	glm::vec3			GetScale() const;
	void				SetMass(float m);
	float				GetMass() const;
 

	void				AddMovementInput(glm::vec3 xyz);
	void				AddRotationInput(glm::vec3 xyz);

	glm::vec3			GetVelocity() const;
	void				SetVelocity(glm::vec3 xyz);
	void				SetForce(glm::vec3 xyz);
	glm::vec3			GetForce() const;

public:

	bool operator==(const Object& other) const {
		return this->id == other.id;
	}
};

