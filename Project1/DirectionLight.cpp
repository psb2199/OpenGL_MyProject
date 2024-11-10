#include "DirectionLight.h"


DirectionLight::DirectionLight(glm::vec3 xyz)
{
	type = "none";
	location = xyz;
	direction = glm::vec3{ 1.0 };
	color = glm::vec3{ 1.f, 1.f, 0.9 };
}

DirectionLight::~DirectionLight()
{
}


glm::vec3 DirectionLight::GetLightColor() const
{
	return color;
}

void DirectionLight::SetDirection(glm::vec3 xyz)
{
	direction = glm::normalize(xyz);
}
glm::vec3 DirectionLight::GetDirection() const
{
	return glm::normalize(direction);
}

void DirectionLight::SetLocation(glm::vec3 xyz)
{
	location = xyz;
}
glm::vec3 DirectionLight::GetLocation() const
{	
	if (attached_obj)
	{
		glm::vec3 new_location = location + attached_obj->GetLocation();
		new_location.y = location.y;

		return new_location;
	}
	else return location ;
}



void DirectionLight::AttachDirectionLight(Object* obj)
{
	attached_obj = obj;
}


void DirectionLight::LightWorks(GLuint Shader)
{
	glm::vec3 Light_direction = GetDirection();
	glUniform3f(glGetUniformLocation(Shader, "LightDir"), Light_direction.x, Light_direction.y, Light_direction.z);

	unsigned int lightColorLocation = glGetUniformLocation(Shader, "lightColor");
	glm::vec3 light_color = GetLightColor();
	glUniform3f(lightColorLocation, light_color.r, light_color.g, light_color.b);


	glm::vec3 light_location = GetLocation();
	glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -100.f, 100.f);
	glm::mat4 lightView = glm::lookAt(light_location, light_location - Light_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	unsigned int ulightSpaceMatrix = glGetUniformLocation(Shader, "lightSpaceMatrix");
	glUniformMatrix4fv(ulightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}

