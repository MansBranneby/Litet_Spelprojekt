#include "DBOHandler.h"

DBOHandler::DBOHandler()
{
	// ANIMATION 1 (Freeway, left)
	for (int i = 0; i < OBJECT_NR_1; i++)
		m_cars1.push_back(new DBGObj(Animation::e_Freeway, false, 60.0f));

	m_cars1[0]->setDrawn(true);
}

DBOHandler::~DBOHandler()
{
	for (int i = 0; i < m_cars1.size(); i++)
		delete m_cars1[i];
}

void DBOHandler::update(float dt)
{
	// Spawn logic

	// Transformation logic
	for (int i = 0; i < m_cars1.size(); i++)
	{
		if (m_cars1[i]->isDrawn()) // If car has spawned
		{
			m_cars1[i]->animate(dt);
		}
	}
}

int DBOHandler::getDBONr()
{
	int drawnObjects = 0;
	for (int i = 0; i < m_cars1.size(); i++)
	{
		if (m_cars1[i]->isDrawn())
			drawnObjects++;
	}	
	return drawnObjects;
}

objectData DBOHandler::getData(int index)
{
	return m_cars1[index]->getData();
}

//void DBOHandler::draw()
//{
//}
