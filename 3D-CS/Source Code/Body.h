#ifndef _BODY_H__
#define _BODY_H__

#include <string>
#include <fstream>
#include <sstream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "Camera.h"
using namespace std;
class Body
{
public:
	GLuint bodyVAO, bodyVBO;

	Body();
	~Body(){};
	void Init(char* path);
	void Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);

public:
	Shader bodyShader;
	GLuint cubemapTexture;
};


#endif