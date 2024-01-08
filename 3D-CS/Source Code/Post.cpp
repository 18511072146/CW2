#include "stdafx.h"
#include "Post.h"

Post::Post()
{
	dis = 0.0f;
	c_Yaw = 0.0f;
	c_Pitch = 0.0f;
	cenx = ceny = 0.0f;
}

void Post::Init()
{
	quad.Init("Quad", "shaders/post/post.vs", "shaders/post/post.frag",
		"resources/NULL", "resources/NULL", "resources/NULL");
}

void Post::Update(GLfloat deltatime, bool shoot)
{
	float move = 0.1f;
	if (shoot)
	{
		if (dis > 0.02f) dis = dis - deltatime * move;
		else
			dis += deltatime*move*1.5;
		 
	}
	else
	{
		dis -= deltatime*move;
		if (dis < 0.0f) dis = 0.0f;
	}
}

void Post::Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view)
{
	glm::mat4 model = glm::mat4();
	glm::mat4 newmodel = glm::mat4();
	glm::vec3 pos = camera.Position;

	model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
	model = glm::rotate(model, glm::radians(90.0f-camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0-camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	
	newmodel = glm::translate(model, glm::vec3(0.02f + dis + cenx, 0.0f + ceny, 1.0f));
	newmodel = glm::scale(newmodel, glm::vec3(0.02f, 0.004f, 1.0f));
	quad.Render(camera, projection, view, newmodel);

	newmodel = glm::translate(model, glm::vec3(-0.02f - dis + cenx, 0.0f + ceny, 1.0f));
	newmodel = glm::scale(newmodel, glm::vec3(0.02f, 0.004f, 1.0f));
	quad.Render(camera, projection, view, newmodel);

	newmodel = glm::translate(model, glm::vec3(0.0f + cenx, 0.02f + dis + ceny, 1.0f));
	newmodel = glm::scale(newmodel, glm::vec3(0.004f, 0.02f, 1.0f));
	quad.Render(camera, projection, view, newmodel);

	newmodel = glm::translate(model, glm::vec3(0.0f + ceny, -0.02f - dis + ceny, 1.0f));
	newmodel = glm::scale(newmodel, glm::vec3(0.004f, 0.02f, 1.0f));
	quad.Render(camera, projection, view, newmodel);
}