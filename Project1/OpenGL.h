#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <iostream>
using namespace std;

enum press { w, a, s, d, KEY_COUNT};
struct vector3 
{ 
	float x, y, z; 

	void print_vec3() 
	{
		cout << " x:" << x << " y:" << y << " z:" << z << endl;
	}
};