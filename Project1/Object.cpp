
#include "Object.h"
#include "ObjectManager.h"


Object::Object(int obj_id, std::string type, glm::vec3 loc, Importer_obj* importer, ObjectManager* objmgr)
{
	Importer_mesh = importer;
	m_objectmgr = objmgr;
	id = obj_id;
	object_type = type;
	mesh = nullptr;

	location = glm::vec3(0.0);
	rotation = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	mass = 1.0;

	AddMovementInput(loc);
	BeginPlayEvent();
}

Object::~Object()
{
}

void Object::BeginPlayEvent()
{
}

void Object::TickEvent(float delta_seconds)
{
	elapesedTime += delta_seconds;
	m_delta_time = delta_seconds;

	if (!setting.isStatic)
	{
		if (setting.EnalbeGravity) { velocity.y -= GRAVITY * delta_seconds; }

		glm::vec3 flatmoveSpeed = { velocity.x, 0.0, velocity.z };
		if (glm::length(flatmoveSpeed) > 0.001)
		{
			SetForce(-flatmoveSpeed * mass * friction);
		}
		else
		{
			velocity.x = 0.0;
			velocity.z = 0.0;
			force = glm::vec3(0.0);
		}

		AddMovementInput(velocity);
	}

	if (setting.EnalbeCollision)
	{
		CheckAllCollisions(m_objectmgr->GetAllObjects());
	}
}

void Object::OverlapedCollisionEvent(Object* collision_obj)
{
}


float Object::GetElapsedTime()
{
	return elapesedTime;
}

float Object::GetDeltaTime()
{
	return m_delta_time;
}

ObjectManager* Object::GetWorld()
{
	return m_objectmgr;
}

void Object::SetCamera(Camera* camera)
{
	if (camera) m_Camera = camera;
	else cout << "There is no Camera" << endl;
}
Camera* Object::GetCamera()
{
	if (m_Camera) return m_Camera;
	else {
		cout << "Camera has not attached" << endl;
		return nullptr;
	}
}

int Object::GetID()
{
	return id;
}
std::string Object::GetObjectType(Object* obj)
{
	return obj->object_type;
}


VertexData* Object::GetMesh()
{
	if (mesh)
	{
		return mesh;
	}
	else return nullptr;
}
void Object::SetMesh(std::string filename)
{
	mesh = Importer_mesh->FindMesh(filename);
	SetCollisionRange();
}
void Object::SetMeshForParticle(GLuint vao, int polycount)
{
	VertexData* newmesh = new VertexData;

	newmesh->filename = "particle";
	newmesh->VAO = vao;
	newmesh->polygon_count = polycount;

	newmesh->VBO = NULL;
	newmesh->texCoordVBO = NULL;
	newmesh->normalVBO = NULL;
	newmesh->tangentVBO = NULL;
	newmesh->bitangentVBO = NULL;
	
	newmesh->vertexs.push_back(glm::vec3(0.0));
	newmesh->texCoords.push_back(glm::vec2(0.0));
	newmesh->normals.push_back(glm::vec3(0.0));

	newmesh->faceIndices.push_back(glm::vec3(0.0));
	newmesh->texCoordIndices.push_back(glm::vec3(0.0));
	newmesh->normalIndices.push_back(glm::vec3(0.0));

	mesh = newmesh;
	SetCollisionRange();
}
Material* Object::GetMaterial()
{
	return m_material;
}
void Object::SetMaterial(std::string filename)
{
	m_material = Importer_mesh->GetMaterial(filename);
}



glm::vec3 Object::GetVelocity() const
{
	return velocity;
}
void Object::SetVelocity(glm::vec3 xyz)
{
	velocity = xyz;
}

void Object::SetForce(glm::vec3 xyz)
{
	force = xyz;

	velocity += force;
}

glm::vec3 Object::GetForce() const
{
	return force;
}

void Object::printObject(Object* obj)
{
	cout << obj->object_type << obj->id << endl;
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

void Object::SetScale(glm::vec3 new_scale)
{
	scale = new_scale;
}
glm::vec3 Object::GetScale() const
{
	return scale;
}

void Object::SetMass(float m)
{
	mass = m;
}
float Object::GetMass() const
{
	return mass;
}




void Object::AddMovementInput(glm::vec3 xyz)
{
	location += xyz;
}
void Object::AddRotationInput(glm::vec3 xyz)
{
	rotation += xyz;
}



void Object::SetCollisionRange()
{
	collision_range.min = glm::vec3(GetMesh()->min_location);
	collision_range.max = glm::vec3(GetMesh()->max_location);
	origin_collision_range = collision_range;
	setting.EnalbeCollision = true;
}

void Object::UpdateCollisionRange()
{
	collision_range.min = origin_collision_range.min * scale + location;
	collision_range.max = origin_collision_range.max * scale + location;
}

bool Object::CheckCollision(const CollisionBox& box1, const CollisionBox& box2)
{
	bool xCollision = (box1.max.x >= box2.min.x) && (box1.min.x <= box2.max.x);
	bool yCollision = (box1.max.y >= box2.min.y) && (box1.min.y <= box2.max.y);
	bool zCollision = (box1.max.z >= box2.min.z) && (box1.min.z <= box2.max.z);

	return xCollision && yCollision && zCollision;
}
void Object::CheckAllCollisions(std::vector<Object*> WorldObjects)
{
	UpdateCollisionRange();

	for (auto& v : WorldObjects)
	{
		if (v != this && v->setting.EnableRendering)
		{
			if (CheckCollision(collision_range, v->GetCollisionRange()))
			{
				if (v->setting.isStatic) AddMovementInput(-velocity);
				OverlapedCollisionEvent(v);
			}

		}
	}

}
CollisionBox Object::GetCollisionRange() const
{
	return collision_range;
}

