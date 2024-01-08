#include "stdafx.h"
#include "Man.h"

// 全局变量，省去Draw_Node里每次传递的麻烦
Camera a_camera;
glm::mat4 a_projection;
glm::mat4 a_view;

bool ManInit = false;

// Here angle is degree
void Rotate(Node *n, float angle, float x, float y, float z)
{
	if (n->constriant)
	{
		if (n->rotate >= n->max && n->dir == 1.0f) n->dir = -1.0f;
		if (n->rotate <= n->min && n->dir == -1.0f) n->dir = 1.0f;
		angle *= n->dir;
	}
	n->rotate += angle;

	glm::mat4 model = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(x, y, z));
	n->model = n->model * model;
}

void Bias_Rotate(Node *n, float angle, float dx, float dy, float dz, float x, float y, float z)
{
	if (n->constriant)
	{
		if (n->rotate >= n->max && n->dir == 1.0f) n->dir = -1.0f;
		if (n->rotate <= n->min && n->dir == -1.0f) n->dir = 1.0f;
		angle *= n->dir;
	}
	n->rotate += angle;

	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(dx, dy, dz));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
	model = glm::translate(model, glm::vec3(-dx, -dy, -dz));
	n->model = n->model * model;
}

void Draw_Node(Node* node, glm::mat4 &model, bool head)
{
	int i = 0;
	while (node != NULL)
	{
		glm::mat4 same_model = model * node->model;
		glm::mat4 ws_model = same_model * node->S;
		if (head==true) node->part->Render(a_camera, a_projection, a_view, ws_model);
		head = true;
		Node* begin = node->chain;
		if (begin != NULL)
		{
			glm::mat4 the_model = same_model;
			Draw_Node(begin, the_model, head);
		}
		node = node->next;
	}
}

Man::Man()
{
	life = 100.0f;
	state = 0;
	jump = 0;
	shoot = 0;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	id = -1;
}

Man::~Man()
{
	for (int i = 0; i < bone_num; i++)
		delete node[i];
}

void Man::SetShoot(bool ifshoot)
{
	shoot = ifshoot;
}

void Man::SetState(bool ifmove)
{
	state = ifmove;
}

void Man::DeduceLife(float de_life)
{
	life -= de_life;
	harm += de_life;
}

void Man::Init()
{
	bone_num = 10;
	body[0].Init("head");
	body[1].Init("zygote");
	body[2].Init("leftshoulder");
	body[3].Init("lefthand");
	body[4].Init("rightshoulder");
	body[5].Init("righthand");
	body[6].Init("leftthigh");
	body[7].Init("leftshank");
	body[8].Init("rightthigh");
	body[9].Init("rightshank");

	if (!ManInit)
		std::cout << "Body Shaders are loaded successfullly! Body initialization complete!" << std::endl;

	// Create the Node bone tree
	for (int i = 0; i < bone_num; i++)
	{
		node[i] = new Node;
		node[i]->part = &body[i];
		node[i]->model = glm::mat4();
	}

	glm::mat4 model = glm::mat4();

	node[0]->S = glm::scale(glm::mat4(), glm::vec3(0.12f, 0.15f, 0.08f));
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, 1.55f, 0.0f));
	node[0]->Assign(NULL, node[1], model, 180.0f, -180.0f, Rotate, NULL);
	node[0]->constriant = false;

	node[1]->S = glm::scale(glm::mat4(), glm::vec3(0.2f, 0.3f, 0.1f));
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.45f, 0.0f));
	node[1]->Assign(NULL, node[2], model, 60.0f, -60.0f, Rotate, NULL);

	node[2]->S = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.12f, 0.06f));
	model = glm::translate(glm::mat4(), glm::vec3(0.25f, 0.18f, 0.0f));
	node[2]->Assign(node[4], node[3], model, -40.0f, -50.0f, NULL, &Bias_Rotate);
	node[2]->BR(node[2], 45.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 0.2f);

	node[3]->S = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.15f, 0.06f));
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.25f, 0.0f));
	node[3]->Assign(NULL, NULL, model, 70.0f, 0.0f, NULL,  &Bias_Rotate);
	node[3]->BR(node[3], -70.0f, 0.0f, 0.15f, 0.0f, 1.0f, 0.1f, 0.3f);

	node[4]->S = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.12f, 0.06f));
	model = glm::translate(glm::mat4(), glm::vec3(-0.25f, 0.18f, 0.0f));
	node[4]->Assign(node[6], node[5], model, 25.0f, 15.0f, NULL, &Bias_Rotate);
	node[4]->BR(node[4], 20.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, -0.2f);

	node[5]->S = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.15f, 0.06f));
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.25f, 0.0f));
	node[5]->Assign(NULL, NULL, model, 110.0f, 0.0f, NULL, &Bias_Rotate);
	node[5]->BR(node[5], -90.0f, 0.0f, 0.15f, 0.0f, 1.0f, -0.2f, -0.1f);

	node[6]->S = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.22f, 0.08f));
	model = glm::translate(glm::mat4(), glm::vec3(0.1f, -0.5f, 0.0f));
	node[6]->Assign(node[8], node[7], model, 25.0f, -20.0f, NULL, &Bias_Rotate);

	node[7]->S = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.22f, 0.08f));
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.38f, 0.0f));
	node[7]->Assign(NULL, NULL, model, 30.0f, 0.0f, NULL, &Bias_Rotate);

	node[8]->S = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.22f, 0.08f));
	model = glm::translate(glm::mat4(), glm::vec3(-0.1f, -0.5f, 0.0f));
	node[8]->Assign(NULL, node[9], model, 25.0f, -20.0f, NULL, &Bias_Rotate);

	node[9]->S = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.22f, 0.08f));
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.38f, 0.0f));
	node[9]->Assign(NULL, NULL, model, 30.0f, 0.0f, NULL, &Bias_Rotate);

	if (!ManInit)
	{
		std::cout << "Body Bones are connected successfullly! Please wait for the process..." << std::endl;
		ManInit = true;
	}

	post.Init();
}

void Man::Update(GLfloat deltatime, GLboolean* keys)
{
	Run(deltatime, keys);
}

void MainMan::Update(GLfloat deltatime, GLboolean* keys)
{
	post.Update(deltatime, shoot);
	Run(deltatime, keys);
}

void Man::Run(GLfloat deltatime, GLboolean *keys)
{
	if (state == 0)
	{
		node[6]->dir = -1.0f;
		node[9]->dir = -1.0f;
		state = 1;
	}

	node[2]->BR(node[2], deltatime*5.0f, 0.0f, 0.1f, 0.0f, 1.0f, -2.0f, 1.0f);
	node[4]->BR(node[4], deltatime*5.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, -0.2f);

	node[6]->BR(node[6], deltatime*30.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f);
	node[7]->BR(node[7], deltatime*20.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f);
	node[8]->BR(node[8], deltatime*30.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f);
	node[9]->BR(node[9], deltatime*20.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f);
}

void Man::Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, Gun &gun)
{
	if (life < 0.0f) return;

	a_camera = camera;
	a_projection = projection;
	a_view = view;

	float n_yaw = 90.0f - front;
	node[0]->R(node[0], n_yaw - node[0]->rotate, 0.0f, 1.0f, 0.0f);
	glm::mat4 model = glm::translate(glm::mat4(), position);
	Draw_Node(node[0], model, true);

	glm::mat4 gun_model = model*node[0]->model*node[1]->model * node[2]->model*node[3]->model;
	gun.Render(camera, projection, view, gun_model);
}

void MainMan::GetPostion(Camera &camera)
{
	float yaw = 90.0f - camera.Yaw;
	position = glm::vec3(
		camera.Position.x, 
		camera.Position.y - 1.63f, 
		camera.Position.z
		);
	node[0]->R(node[0], yaw-node[0]->rotate, 0.0f, 1.0f, 0.0f);
}

void MainMan::GivePosition(Camera &camera)
{
	camera.Position = position;
}

void MainMan::Render(Camera &camera, glm::mat4 &projection, glm::mat4 &view, Gun &gun)
{
	front = camera.Yaw;
	a_camera = camera;
	a_projection = projection;
	a_view = view;

	glm::mat4 model = glm::translate(glm::mat4(), position);
	Draw_Node(node[0], model, false);

	glm::mat4 gun_model = model*node[0]->model*node[1]->model * node[2]->model*node[3]->model;
	gun.Render(camera, projection, view, gun_model);
	post.Render(camera, projection, view);
}