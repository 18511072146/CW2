#ifndef _POST_H__
#define _POST_H__
#include "Shape.h"
#include "Camera.h"
class Post
{
public:
	Post();
	void Init();
	void Update(GLfloat deltatime, bool shoot);
	void Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view);
	void Print(){ cout << dis<<endl; }
private:
	Shape quad;
	float dis, c_Yaw, c_Pitch;
	float cenx, ceny;
};

#endif