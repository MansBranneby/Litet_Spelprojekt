#pragma once

#include "DBGObj.h"

#define OBJECT_NR_1 10

class DBOHandler
{
private:
	std::vector<DBGObj*> m_cars1;
	float m_bigTimer;
	float m_lastSpawn;
	int m_drawnCars;
public:
	DBOHandler();
	~DBOHandler();

	void update(float dt);
	int getDBONr();
	objectData getData(int index);
	bool isDrawn(int index);
	//void draw();
};

