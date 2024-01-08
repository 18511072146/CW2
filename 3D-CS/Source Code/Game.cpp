#include "stdafx.h"
#include "iostream"
#include "vector"
#include "stdio.h"
#include<time.h>

#include "Game.h"
extern float dx, dy;

float clickTime = 0.0f; // the shooting speed of the bullet
void ShootDegreeUpdate(GLfloat deltatime, bool shoot);

Game::Game(GLuint width, GLuint height)
{
	Width = width;
	Height = height;
}

Game::~Game()
{
	data.DeletePlayer(&man);
}

void Game::Init()
{
	skybox.Init();

	map.Init();

	man.Init();

	for (int i = 0; i < costar_num; i++)
		costar[i].Init();

	gun.Init();

	data.Init(costar, &man, map, player_num);
	man.GivePosition(camera);

	std::cout << "All initialization work has been done! Enjoy the game now! " << std::endl;
}

void Game::ProcessInput(GLfloat deltatime, bool shoot)
{
	glm::vec3 pos = camera.Position;

	man.SetState(false);
	GLfloat cameraSpeed = camera.cameraspeed * deltatime;
	if (keys[GLFW_KEY_W])
	{
		camera.Position += cameraSpeed * camera.Front;
		man.SetState(true);
	}
	if (keys[GLFW_KEY_S])
	{
		camera.Position -= cameraSpeed * camera.Front;
		man.SetState(true);
	}
	if (keys[GLFW_KEY_A])
	{
		man.SetState(true);
		camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	}
	if (keys[GLFW_KEY_D])
	{
		camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
		man.SetState(true);
	}
	
	camera.Position.y = 0.0f;
	if (map.CheckCollision(camera.Position, costar, player_num, false))
	{
		float newx = camera.Position.x; float newz = camera.Position.z;
		camera.Position = pos;
		camera.Position.y = 0.0f;
		camera.Position.x = newx;
		if (map.CheckCollision(camera.Position, costar, player_num, false)) camera.Position.x = pos.x;
		camera.Position.z = newz;
		if (map.CheckCollision(camera.Position, costar, player_num, false)) camera.Position.z = pos.z;
	}
	camera.Position.y = 1.63f;

	if (shoot)
	{
		if (clickTime > 0.5)
		{
			glm::float32 dis = 0.2;
			glm::vec3 pos = camera.Position;
			while (!map.CheckCollision(pos, costar, player_num, true))
				pos += dis*camera.Front;
		}
		clickTime = 0.0f;
	}
	else clickTime += deltatime;
}

void Game::Update(GLfloat deltatime, int left_button)
{
	// Control the camera
	camera.Position.y = 1.63f;

	// Here update the main man parameters.
	if (left_button) man.SetShoot(true);
	else man.SetShoot(false);
	
	// Update the move degree of the shoot
	ShootDegreeUpdate(deltatime, left_button);

	man.GetPostion(camera);
	man.Update(deltatime, keys);

	data.ReadData();
	player_num = data.ProcessResult(&man, costar);
	data.UploadData(&man, costar, player_num);

	// Here update the costar parameters.
	for (int i = 0; i < player_num; i++)
		costar[i].Update(deltatime, keys);
}

void ShootDegreeUpdate(GLfloat deltatime, bool left_button)
{
	srand((unsigned)time(NULL));
	float angle = (rand() % 60 + 60)*0.017;
	if (left_button)
	{
		if (dx > 6.0f) dx -= deltatime *25.0f*cos(angle);
		if (dy > 6.0f) dy -= deltatime *25.0f*sin(angle);
		dx += deltatime * 25.0f*cos(angle);
		dy += deltatime * 25.0f*sin(angle);
	}
	else if (dy > 0.0f)
	{
		if (dx > 0) dx -= deltatime *25.0f; else dx += deltatime *25.0f;
		dy -= deltatime *25.0f;
	}
	// if (dx < 0.0f) dx = 0.0f;
	if (dy < 0.0f) dy = 0.0f;
}

void Game::Render()
{
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	skybox.Render(camera, projection);

	glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
	map.Render(camera, projection, view);

	man.Render(camera, projection, view, gun);

	for (int i = 0; i < player_num; i++)
		costar[i].Render(camera, projection, view, gun);

}

