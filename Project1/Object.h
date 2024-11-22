#pragma once

#include "OpenGL.h"

#include "Importer.h"
#include "Camera.h"

#define GRAVITY	1
class ObjectManager;

struct Setting {
	bool				isStatic{ false };
	bool				EnalbeGravity{ false };
	bool				cast_shadow{ true };
	bool				EnalbeCollision{ false };
	bool				EnableRendering{ true };

	bool				EnalbeTwoFace{ false };
};


class Object
{	
	int					id;
	int					object_type;
	VertexData*			mesh;
	CollisionBox		origin_collision_range;
	CollisionBox		collision_range;
	prev_Material*			m_material;

	glm::vec3			location;
	glm::vec3			rotation;
	glm::vec3			scale;
	float				mass;
	glm::vec3			velocity;
	glm::vec3			force;
	float				friction{ 0.1 };


	bool				isOverlapped{ false };

	float				elapesedTime{ 0.0 };
	float				m_delta_time;

	Camera*				m_Camera;
	Importer*		Importer_mesh;
	ObjectManager*		m_objectmgr;
	
	void				SetCollisionRange();
	void				UpdateCollisionRange();
	bool				CheckCollision(const CollisionBox& box1, const CollisionBox& box2);
	void				CheckAllCollisions(std::vector<Object*> WorldObjects);
public:
	Setting				setting;

	Object() = default;
	Object(int obj_id, int type, glm::vec3 loc, Importer* importer, ObjectManager* objmgr);
	virtual ~Object();

	virtual void		BeginPlayEvent();
	virtual void		TickEvent(float delta_seconds);
	virtual void		OverlapedCollisionEvent(Object* collision_obj);

	float				GetElapsedTime();
	float				GetDeltaTime();
	ObjectManager*		GetWorld();

	CollisionBox		GetCollisionRange() const;
	void				SetCamera(Camera* camera);
	Camera*				GetCamera();

	int					GetID();
	int					GetObjectType(Object* obj);

	VertexData*			GetMesh();
	void				SetMesh(std::string filename);
	void				SetMesh(VertexData* data);
	prev_Material*			GetMaterial();
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

	static void			printObject(Object* obj);
public:

	bool operator==(const Object& other) const {
		return this->id == other.id;
	}
};

