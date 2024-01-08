#ifndef _SKYBOX_H__
#define _SKYBOX_H__

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

class SkyBox
{
public:
	GLuint skyboxVAO, skyboxVBO;

	SkyBox();
	~SkyBox(){};
	void Init();
	void Render(Camera &camera, glm::mat4 &projection);

public:
	Shader skyboxShader;
	GLuint cubemapTexture;
};

#endif