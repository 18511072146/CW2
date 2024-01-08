#ifndef _SHAPE_H__
#define _SHAPE_H__

#include <GL/glew.h>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include <string>
using namespace std;

class Shape
{
public:
	Shape();
	void Init(string thetype, GLchar* vspath, GLchar* fragpath, char* diffusemappath, char* normalmappath, char*depthmappath);
	void SetLightPos(glm::vec3 &pos);
	void SetHeightScale(GLfloat scale);
	void Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);
private:
	glm::vec3 lightPos;
	GLuint VBO, VAO;
	Shader shapeShader;
	GLuint diffuseMap;
	GLuint normalMap;
	GLuint depthMap;
	string type;
	float height_scale;
};

#endif