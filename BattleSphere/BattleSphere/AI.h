#pragma once
#include "Graph.h"
#include "DX.h"
#include <cmath>
struct mission {
	XMVECTOR pos;
	int index;
};
class AI
{
private:
	std::vector<XMVECTOR> currentPath;
public:
	mission setMission(std::vector<XMVECTOR> path);
	mission update(float dt, float movementSpeed, mission currentMission);
	XMVECTOR getAIRotation(mission currentMission);
	AI();
	~AI();
};

