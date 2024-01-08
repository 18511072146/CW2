#ifndef _MAP_H__
#define _MAP_H__

#include "Shape.h"
#include "Man.h"

struct point
{
	float x, z;
};

class Map
{
public:
	Map();
	void Init();
	bool CheckCollision(glm::vec3 pos, Man* costar, int costar_num, bool bullet);
	void Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view);
private:
	GLfloat size;
	Shape shape[10];
	point p[6];
};

#endif