#ifndef _SHADER_H__
#define _SHADER_H__

#include <string>
#include <fstream>
#include <sstream>
#include "vector"
#include <iostream>
#include <GL/glew.h>
#include <SOIL.h>

class Shader
{
public:
	Shader(){};
	// 程序ID
	GLuint Program;
	// 构造器读取并构建着色器
	void LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用程序
	void Use();
};

GLuint loadCubemap(std::vector<const GLchar*> faces);
#endif