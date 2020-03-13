#include "AI.h"



mission AI::setMission(std::vector<XMVECTOR> path)
{
	mission newMission;
	currentPath = path;
	newMission.pos = path[path.size() - 1];
	newMission.index = (int)path.size() - 1;
	return newMission;
}

mission AI::update(float dt, float movementSpeed, mission currentMission)
{
	if (1 > currentMission.index)
	{
		return currentMission;
	}
	if (XMVectorGetX(XMVector3Length(currentPath[currentMission.index - 1] - currentMission.pos)) < dt * movementSpeed)
	{
		currentMission.index--;
		currentMission.pos = currentPath[currentMission.index];
	}
	else {

		currentMission.pos += XMVector3Normalize((currentPath[currentMission.index - 1] - currentMission.pos)) * dt * movementSpeed;
	}
	return currentMission;
}

XMVECTOR AI::getAIRotation(mission currentMission)
{
	if (1 <= currentMission.index)
	{
		XMVECTOR delta = XMVector3Normalize(currentPath[currentMission.index - 1] - currentMission.pos);
		return delta;
	}
	return XMVectorSet(1,0,0,0);
}

AI::AI()
{
}

AI::~AI()
{
}
