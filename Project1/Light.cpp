#include "Light.h"


Light::Light(glm::vec3 xyz)
{
	type = "none";
	location = xyz;
	rotation = glm::vec3{ 0 };
	color = glm::vec3{ 1.f, 1.f, 0.9 };
	distance = 200.f;
}

Light::~Light()
{
}

glm::vec3 Light::GetLocation() const
{
	return location;
}

glm::vec3 Light::GetLightColor() const
{
	return color;
}

float Light::GetLightDistance() const
{
	return distance;
}

void Light::LightWorks(GLuint Shader)
{
	unsigned int lightPosLocation = glGetUniformLocation(Shader, "lightPos");
	glm::vec3 light_location = GetLocation();
	glUniform3f(lightPosLocation, light_location.x, light_location.y, light_location.z);

	unsigned int lightColorLocation = glGetUniformLocation(Shader, "lightColor");
	glm::vec3 light_color = GetLightColor();
	glUniform3f(lightColorLocation, light_color.r, light_color.g, light_color.b);

	unsigned int lightDistanceLocation = glGetUniformLocation(Shader, "lightDistance");
	glUniform1f(lightDistanceLocation, GetLightDistance());

	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -100.f, 20.f);
	glm::mat4 lightView = glm::lookAt(light_location, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	unsigned int ulightSpaceMatrix = glGetUniformLocation(Shader, "lightSpaceMatrix");
	glUniformMatrix4fv(ulightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}
