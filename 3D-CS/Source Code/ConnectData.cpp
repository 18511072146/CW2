#include "stdafx.h"
#include "ConnectData.h"
#include "iostream"
#include<WinSock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")  


DataBase::DataBase()
{
	conn = NULL;
	pghost = "XXX.XX.XXX.XXX";
	pgport = "XXXX";
	dbname = "CSplayer";
	user = "postgres";
	pwd = "XXXXXX";
	for (int i = 0; i < MAX_PLAYER;i++) id[i] = 0;
}

void DataBase::Init(char* n_pghost, char* n_pgport, char* n_dbname, char* n_user, char* n_pwd)
{
	pghost = n_pghost;
	pgport = n_pgport;
	dbname = n_dbname;
	user = n_user;
	pwd = n_pwd;
	ConnectToDB(conn);
}

bool DataBase::ReadData()
{
	query = "select * from player";
	res = PQexec(const_cast<PGconn *>(conn), query);
	if (res == NULL)
	{
		std::cout << "Meet error when executing SQL order to database! " << std::endl;
		return false;
	}
	else
	{
		// 输出记录
		int tuple_num = PQntuples(res);
		if (tuple_num > MAX_PLAYER - 1)
		{
			std::cout << "There has been too many players in the server." << std::endl;
			std::cout << "Please waiting for a while..." << std::endl;
			return false;
		}
	}

	return true;
}

void DataBase::Init(Man* costar, MainMan* man, Map &map, int &num)
{
	if (!ConnectToDB(conn))
	{
		std::cout << "Failed to connect to the database! Check your internet connection! " << std::endl;
		return;
	}

	float px, py, pz;
	ReadData();
	num = ProcessResult(man, costar);

	// Create data for MainMan
	if (man->id == -1)
	{
		int k = 0;
		while (id[k] == 1) k++;
		man->id = k;
	}
	srand((int)time(NULL));
	int xory = rand() % 2;
	while (map.CheckCollision(man->position, costar, num, false))
	{
		if (xory == 1)
			man->position.x = rand() % 32 - 16;
		else
			man->position.z = rand() % 32 - 16;
	}

	stringstream ss;
	string str_id, str_px, str_py, str_pz;
	ss << man->id; ss >> str_id; ss.clear();
	ss << man->position.x; ss >> str_px; ss.clear();
	ss << man->position.y; ss >> str_py; ss.clear();
	ss << man->position.z; ss >> str_pz; ss.clear();
	string s = "insert into player values (";
	s += str_id + ",100.0, 0, 0.0," + str_px + "," + str_py + "," + str_pz + ")";
	query = s.c_str();
	PQexec(conn, query);
}

bool DataBase::ConnectToDB(PGconn *&n_conn)
{
	//pgoptions、pgtty参数默认为NULL
	char *pgoptions, *pgtty;
	pgoptions = NULL;
	pgtty = NULL;

	n_conn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, dbname, user, pwd);
	if (PQstatus(n_conn) == CONNECTION_BAD) // or conn==NULL 
	{
		std::cout << "Connection database " << dbname << " failed!" << std::endl;
		std::cout << PQerrorMessage(n_conn) << std::endl;
		return false;
	}
	else
	{
		std::cout << "Connection databse " << dbname << " success!" << std::endl;
		return true;
	}
}

int DataBase::ProcessResult(MainMan* man, Man* costar)
{
	int tuple_num = PQntuples(res);
	int field_num = PQnfields(res);

	// Here set field_num as 7 since we assume the format is:
	// id, life, state, front, px, py, pz
	// id: random number between 0 and 9                 life: from 0 to 100
	// state: jump or run or others                      front: from 0 to 360
	// px, py, pz: the position of this costar
	field_num = 7;
	char* value;
	int k = 0;
	for (int i = 0; i<tuple_num; ++i)
	{
		int c_id = atoi(PQgetvalue(res, i, 0));
		float px, py, pz;
		px = atof(PQgetvalue(res, i, 4));
		py = atof(PQgetvalue(res, i, 5));
		pz = atof(PQgetvalue(res, i, 6));
		if (man->id != c_id)
		{
			costar[k].id = c_id; id[c_id] = 1;
			costar[k].life = atof(PQgetvalue(res, i, 1));
			costar[k].state = atoi(PQgetvalue(res, i, 2));
			costar[k].front = atof(PQgetvalue(res, i, 3));
			costar[k].position = glm::vec3(px, py, pz);
			k++;
		}
		else
		{
			man->life = atof(PQgetvalue(res, i, 1));
			man->state = atoi(PQgetvalue(res, i, 2));
		}
	}
	PQclear(res);

	return k;
}

void DataBase::UploadData(MainMan* man, Man* costar, int player_num)
{
	stringstream ss;
	string str_id, str_life, str_state, str_front, str_px, str_py, str_pz;
	ss << man->id; ss >> str_id; ss.clear();
	ss << man->life; ss >> str_life; ss.clear();
	ss << man->front; ss >> str_front; ss.clear();
	ss << man->state; ss >> str_state; ss.clear();
	ss << man->position.x; ss >> str_px; ss.clear();
	ss << man->position.y; ss >> str_py; ss.clear();
	ss << man->position.z; ss >> str_pz; ss.clear();
	string s = "update player set ";
	s += " life = "+str_life+", state ="+str_state+", front = "+ str_front+", px = "+str_px+
		", py = "+str_py +", pz = "+str_pz+" where id = " + str_id;
	query = s.c_str();
	PQexec(conn, query);

	for (int i = 0; i < player_num; i++)
	{
		if (costar[i].harm != 0)
		{
			float life = costar[i].life - costar[i].harm;
			ss << life; ss >> str_life; ss.clear();
			ss << costar[i].id; ss >> str_id; ss.clear();
			string s = "update player set ";
			s += "life = " + str_life + " where id = " + str_id;
			query = s.c_str();
			PQexec(conn, query);
		}
		costar[i].harm = 0;
	}
}

void DataBase::DeletePlayer(MainMan* man)
{
	stringstream ss;
	string str_id;
	ss << man->id; ss >> str_id; ss.clear();
	string s = "delete from player where id = ";
	s += str_id;
	query = s.c_str();
	PQexec(conn, query);
}
