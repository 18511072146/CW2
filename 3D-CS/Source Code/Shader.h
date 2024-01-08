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
	// ����ID
	GLuint Program;
	// ��������ȡ��������ɫ��
	void LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ʹ�ó���
	void Use();
};

GLuint loadCubemap(std::vector<const GLchar*> faces);
#endif