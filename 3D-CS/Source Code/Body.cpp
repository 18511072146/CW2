#include "stdafx.h"
#include "Body.h"

GLfloat bodyVertices[] = {
	// Positions          
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

Body::Body()
{

}

void Body::Init(char* part)
{
	// Setup skybox VAO
	glGenVertexArrays(1, &bodyVAO);
	glGenBuffers(1, &bodyVBO);
	glBindVertexArray(bodyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bodyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bodyVertices), &bodyVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	std::vector<const GLchar*> faces;
	string spart = (string)part;
	string right = "resources/body/"+spart+"/right.jpg";
	string left = "resources/body/" + spart + "/left.jpg";
	string top = "resources/body/" + spart + "/top.jpg";
	string bottom = "resources/body/" + spart + "/bottom.jpg";
	string back = "resources/body/" + spart + "/back.jpg";
	string front = "resources/body/" + spart + "/front.jpg";
	
	faces.push_back(right.c_str());
	faces.push_back(left.c_str());
	faces.push_back(top.c_str());
	faces.push_back(bottom.c_str());
	faces.push_back(back.c_str());
	faces.push_back(front.c_str());
	cubemapTexture = loadCubemap(faces);

	bodyShader.LoadShader("shaders/body/body.vs", "shaders/body/body.frag");
	
	bodyShader.Use();
	glUniform1i(glGetUniformLocation(bodyShader.Program, "bodybox"), 0);
}

void Body::Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model)
{
	glEnable(GL_DEPTH_TEST);

	// Draw skybox as last
	bodyShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(bodyShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(bodyShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(bodyShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// skybox cube
	glBindVertexArray(bodyVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}