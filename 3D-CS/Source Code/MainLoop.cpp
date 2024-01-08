// MainLoop.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include "Game.h"
#include<gl/glut.h>
int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

// The angle to rotate the camera
GLfloat yaw, pitch;
float dx = 0.0f, dy = 0.0f;
float ryaw, rpitch;

// Centre for mouse comparation
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
GLfloat aspect = 45.0f;
int left_button;
Game NewGame(SCREEN_WIDTH, SCREEN_HEIGHT);

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void WindowUpdate();

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	std::cout << "Startign GLFW successfully!" << std::endl;

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CS_GAME", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Cursor can be seen/ not be seen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Time distance
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	NewGame.Init();

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		NewGame.ProcessInput(deltaTime, left_button);
		// Update Game state
		NewGame.Update(deltaTime, left_button);
		// Both press and move of mouse will affect.
		WindowUpdate();

		// Render
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		NewGame.Render();

		if (NewGame.man.life < 0.0) break;

		// Render
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}



// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		glfwTerminate();
		exit(0);
	}

	if (action == GLFW_PRESS)
		NewGame.keys[key] = true;
	else if (action == GLFW_RELEASE)
		NewGame.keys[key] = false;
}

// Is called whenever the mouse is moved or pressed
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标的范围是从下往上的
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	ryaw = yaw + dx; rpitch = pitch + dy;
	glm::vec3 front;
	front.x = cos(glm::radians(rpitch)) * cos(glm::radians(ryaw));
	front.y = sin(glm::radians(rpitch));
	front.z = cos(glm::radians(rpitch)) * sin(glm::radians(ryaw));
	NewGame.camera.Front = glm::normalize(front);
	NewGame.camera.Yaw = ryaw;
	NewGame.camera.Pitch = rpitch;
}

// Is called whenever the scroll is rolling
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (aspect >= 1.0f && aspect <= 45.0f)
		aspect -= yoffset;
	if (aspect <= 1.0f)
		aspect = 1.0f;
	if (aspect >= 45.0f)
		aspect = 45.0f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		left_button = true;
		break;
	default:
		break;
	}

	if (action == GLFW_RELEASE) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		left_button = false;
		break;
	default:
		break;
	}

	return;
}

void WindowUpdate()
{
	ryaw = yaw + dx; rpitch = pitch + dy;
	glm::vec3 front;
	front.x = cos(glm::radians(rpitch)) * cos(glm::radians(ryaw));
	front.y = sin(glm::radians(rpitch));
	front.z = cos(glm::radians(rpitch)) * sin(glm::radians(ryaw));
	NewGame.camera.Front = glm::normalize(front);
	NewGame.camera.Yaw = ryaw;
	NewGame.camera.Pitch = rpitch;
}