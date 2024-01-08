#include <libpq-fe.h>
#include "Man.h"
#include "Map.h"

#define MAX_PLAYER 10

class DataBase
{
public:
	int id[MAX_PLAYER];
	PGconn *conn;
	DataBase();
	bool con;
	void Init(Man* costar, MainMan* man, Map &map, int &num);
	void Init(char* n_pghost, char* pgport, char* dbname, char* user, char*pwd);
	bool ReadData();
	bool ConnectToDB(PGconn *&conn);
	void UploadData(MainMan* man, Man* costar, int player_num);
	void DeletePlayer(MainMan* man);
	int ProcessResult(MainMan* man, Man* costar);
private:
	char *pghost;
	char *pgport;
	char *dbname;
	char *user;
	char *pwd;

	const char *query;
	PGresult* res;
};
