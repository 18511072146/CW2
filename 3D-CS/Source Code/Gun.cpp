#include "stdafx.h"
#include "Gun.h"

Gun::Gun()
{
	// 0:pistol  1:carbin
	type = 0;
	magazine = 7;
	bullet = 21;
	// 0: shot   1: not shot
	state = true;
}

void Gun::Init()
{
	std::cout << "Gun Model is being loaded. Pleasing waiting..." << std::endl;
	gun.LoadShader("shaders/gun/gun.vs", "shaders/gun/gun.frag");
	gun_model.loadModel("resources/pistol/pistol.obj");
	std::cout << "Gun Model has been loaded!" << std::endl;

	model = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.12f, 0.15f, 0.21f));
	model = glm::scale(model, glm::vec3(0.0004f, 0.0004f, 0.0004f));
}

void Gun::Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &man_model)
{
	glEnable(GL_DEPTH_TEST);
	gun.Use();   // <-- Don't forget this one!
	// Transformation matrices
	glUniformMatrix4fv(glGetUniformLocation(gun.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(gun.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 gmodel = man_model*model;
	glUniformMatrix4fv(glGetUniformLocation(gun.Program, "model"), 1, GL_FALSE, glm::value_ptr(gmodel));

	gun_model.Draw(gun);
};
