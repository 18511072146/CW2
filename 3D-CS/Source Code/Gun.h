#ifndef _GUN_H__
#define _GUN_H__

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shape.h"
#include "Model.h"

class Gun
{
public:
	Gun();
	void Init();
	void Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);

private:
	Shader gun;
	Model gun_model;
	glm::mat4 model;
	int type;
	int magazine;
	int bullet;
	bool state;
};

#endif