#include "stdafx.h"
#include "Map.h"
#include "stdlib.h"
#include "iostream"
// This defines the circular radius of human.
float radius = 0.33f;

bool CheckCubeCollision(float x, float z, float x1, float x2, float z1, float z2);
bool CheckPrismCollision(float x, float z, point p1, point p2, point p3);
bool LineCollision(point p1, point p2, point p3, point p);
bool CheckShootCollision(glm::vec3 pos, Man* costar, int costar_num, bool bullet);

Map::Map()
{
	size = 1.0f;

	p[0].x = 1.0 + radius; p[0].z = 16.0;
	p[1].x = -1.0 - radius; p[1].z = 16.0;
	p[2].x = 0; p[2].z = 13.7-radius;

	p[3].x = 16.0; p[3].z = 16.0;
	p[4].x = 14 - radius; p[4].z = 16.0;
	p[5].x = 16.0; p[5].z = 14 - radius;
}

void Map::Init()
{
	std::cout << "Map is being initialized! Pleasing waiting..." << std::endl;

	shape[0].Init("Quad", "shaders/wall/aroundWall.vs", "shaders/wall/aroundWall.frag",
		"resources/Wall/AroundWall.jpg", "resources/Wall/AroundWall-N.jpg", "resources/Wall/AroundWall-D.jpg");
	shape[1].Init("Cube", "shaders/wall/aroundWall.vs", "shaders/wall/aroundWall.frag",
		"resources/Wall/Block.jpg", "resources/Wall/Block-N.jpg", "resources/Wall/Block-D.jpg");
	shape[2].Init("Prism", "shaders/wall/aroundWall.vs", "shaders/wall/aroundWall.frag",
		"resources/Wall/Sharp.jpg", "resources/Wall/Sharp-N.jpg", "resources/Wall/Sharp-D.jpg");
	shape[3].Init("Prism", "shaders/wall/aroundWall.vs", "shaders/wall/aroundWall.frag",
		"resources/Wall/AroundWall.jpg", "resources/Wall/AroundWall-N.jpg", "resources/Wall/AroundWall-D.jpg");
	shape[4].Init("Cube", "shaders/wall/aroundWall.vs", "shaders/wall/aroundWall.frag",
		"resources/Wall/Brick.jpg", "resources/Wall/Brick-N.jpg", "resources/Wall/Brick-D.jpg");
	shape[5].Init("Quad", "shaders/wall/aroundWall.vs", "shaders/wall/aroundWall.frag",
		"resources/Wall/Floor.jpg", "resources/Wall/Floor-N.jpg", "resources/Wall/Floor-D.jpg");

	std::cout << "Map is initialized successfully!" << std::endl;
}

bool Map::CheckCollision(glm::vec3 pos, Man *costar, int costar_num, bool bullet)
{
	float x = pos.x; float z = pos.z; float y = pos.y;
	if (y<-0.01 || y>5) return true;
	bool collision = false;

	int a[4] = { 1, 1, -1, -1 };
	int b[4] = { 1, -1, 1, -1 };
	float size = 16.0 - radius;
	// Whether it is inside the box
	if (x > 0-size&&x < size&&z>0-size&& z < size)
	{
	}
	else return true;

	for (int i = 0; i < 4; i++)
	{
		// The four big block
		if (CheckCubeCollision(x, z, a[i] * 2.5f, a[i] * 9.5f, b[i] * 2.5f, b[i] * 9.5f))
		{
			collision = true; break;
		}
		if (x*x + z*z>49)
		{
			// This small block
			if (CheckCubeCollision(x, z, a[i] * 9.5f, a[i] * 12.0f, b[i] * 9.0f, b[i] * 9.5f)
				&& y>-0.01 && y<2.0)
			{
				collision = true; break;
			}
			
			if (i % 2 == 0)
			{
				if (CheckCubeCollision(x, z, a[i] * 13.6f, a[i] * 16.5f, 0.2f, 0 - 0.2f)
					&& y>-0.01 && y<1.5) 
					collision = true;
				p[0].z = -p[0].z; p[1].z = -p[1].z; p[2].z = -p[2].z;
				// The two prisms on z-axis
				if (CheckPrismCollision(x, z, p[0], p[1], p[2])) collision = true;
			}
			
			// Four prisms on the corner
			if (i == 2)
			{
				p[3].x = -p[3].x; p[4].x = -p[4].x; p[5].x = -p[5].x;
			}
			p[3].z = -p[3].z; p[4].z = -p[4].z; p[5].z = -p[5].z;
			if (CheckPrismCollision(x, z, p[3], p[4], p[5])) collision = true;
		}
	}

	if (CheckShootCollision(pos, costar, costar_num, bullet)) collision = true;

	return collision;
}

void Map::Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view)
{
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 model = glm::mat4();
		model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(16.0f, 5.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 16.0f));
		shape[0].Render(camera, projection, view, model);

		model = glm::mat4();
		model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(16.0f, 5.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 16.0f));
		shape[0].Render(camera, projection, view, model);
		
		if (i % 2 == 0)
		{
			model = glm::mat4();
			model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-14.7f, 0.7f, 0.0f));
			model = glm::scale(model, glm::vec3(2.61f, 1.4f, 0.4f));  // Here we enlarge 2.5->2.51 to avoid overlap
			shape[1].Render(camera, projection, view, model);
		}

		if (i % 2 == 0)
		{
			model = glm::mat4();
			model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0f, 5.0f, 5.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.21f));// Here we enlarge 3.2->3.21 to avoid overlap
			shape[2].Render(camera, projection, view, model);
		}

		model = glm::mat4();
		model = glm::rotate(model, glm::radians(90.0f * (i - 0.5f)), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 2.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 10.85f));
		shape[3].Render(camera, projection, view, model);

		model = glm::mat4();
		GLfloat k = -1.0f;
		if (i >= 2) k = 1.0f;
		model = glm::translate(model, glm::vec3((i % 2 - 0.5) * 2 * 10.75f, 1.0f, k*9.25f));
		model = glm::scale(model, glm::vec3(2.51f, 2.0f, 0.4f));  // Here we enlarge 2.5->2.51 to avoid overlap
		shape[1].Render(camera, projection, view, model);

		model = glm::mat4();
		model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 5.0f, 7.0f));
		model = glm::translate(model, glm::vec3(0.85f, 0.5f, 0.85f));
		shape[4].Render(camera, projection, view, model);
	}

	glm::mat4 model = glm::mat4();
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(32.0f, 32.0f, 1.0f));
	shape[5].Render(camera, projection, view, model);
}

bool CheckCubeCollision(float x, float z, float x1, float x2, float z1, float z2)
{
	bool col = false;
	bool inx = false, inz = false;
	if (x1>=x2) 
	if ((x1 + radius - x)*(x2 - radius - x) < 0) inx = true;
	if (x1<x2)
	if ((x1 - radius - x)*(x2 + radius - x) < 0) inx = true;

	if (z1 >= z2)
	if ((z1 + radius - z)*(z2 - radius - z) < 0) inz = true;
	if (z1<z2)
	if ((z1 - radius - z)*(z2 + radius - z) < 0) inz = true;

	if (inx&&inz) col = true;
	return col;
}

bool CheckPrismCollision(float x, float z, point p1, point p2, point p3)
{
	bool col = true;
	point p;
	p.x = x; p.z = z;
	if (!LineCollision(p1, p2, p3, p)) col = false;
	if (!LineCollision(p1, p3, p2, p)) col = false;
	if (!LineCollision(p3, p2, p1, p)) col = false;
	return col;
}

bool LineCollision(point p1, point p2, point p3, point p)
{
	float a = p2.z - p1.z;
	float b = p1.x - p2.x;
	float c = p2.x*p1.z - p1.x*p2.z;
	float d1 = a*p3.x + b*p3.z + c;
	float d2 = a*p.x + b*p.z + c;
	if (d1*d2 >= 0) return true;
	else return false;
}

bool CheckShootCollision(glm::vec3 pos, Man* costar, int costar_num, bool bullet)
{
	float x = pos.x; float z = pos.z; float y = pos.y;
	bool col = false;
	float max_dis = radius*radius * 4;

	srand((unsigned)time(NULL));
	for (int i = 0; i < costar_num; i++)
	{
		float dx = x - costar[i].position.x;
		float dz = z - costar[i].position.z;
		float d = dx*dx + dz*dz;
		if (d < max_dis && !bullet)
		{
			col = true;
			break;
		}
		if (bullet)
		{
			if (y > 1.4&&y < 1.7 && d<max_dis / 4)
			{
				costar[i].DeduceLife(60.0 + rand() % 20);
				col = true; break;
			}
			if (y>0.8&&y < 1.4&&d < max_dis)
			{
				costar[i].DeduceLife(30.0 + rand() % 20);
				col = true; break;
			}
			if (y < 0.8&&d < max_dis*0.6)
			{
				costar[i].DeduceLife(20.0 + rand() % 10);
				col = true; break;
			}
		}
	}
	return col;
}