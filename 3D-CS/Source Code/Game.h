#ifndef _GAME_H__
#define _GAME_H__

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "SkyBox.h"
#include "Map.h"
#include "Man.h"
#include "ConnectData.h"

#define costar_num 10

class Game
{
public:
	GLboolean keys[1024];
	GLuint Width, Height;

	int player_num;
	Camera camera;
	SkyBox skybox;
	Map map;
	MainMan man;
	Man costar[costar_num];
	Gun gun;
	DataBase data;

	~Game();
	Game(GLuint width, GLuint height);

	//初始化游戏（加载着色器，纹理等）
	void Init();
	//游戏循环
	void ProcessInput(GLfloat deltatime, bool shoot);
	void Update(GLfloat deltatime, int left_button);
	void Render();
};

#endif
