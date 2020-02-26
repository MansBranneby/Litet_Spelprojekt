#include "DBOHandler.h"

DBOHandler::DBOHandler()
{
	// ANIMATION 1 (Freeway, left)
	for (int i = 0; i < OBJECT_NR_1; i++)
		m_cars1.push_back(new DBGObj(Animation::e_Freeway, false, 60.0f));

	m_cars1[0]->setDrawn(true);
	m_drawnCars = 1;
	m_lastSpawn = 0.0f;
}

DBOHandler::~DBOHandler()
{
	for (int i = 0; i < m_cars1.size(); i++)
		delete m_cars1[i];
}

void DBOHandler::update(float dt)
{
	// Spawn logic
	m_lastSpawn += dt;

	if (m_lastSpawn >= 1.0f)
		m_bigTimer += dt;
	if (m_lastSpawn >= 1.0f && m_bigTimer >= 1.0f)
	{
		m_bigTimer = 0.0f;
		float random = rand() % 10;
		if (random + m_lastSpawn >= 9.0f && m_drawnCars < OBJECT_NR_1)
		{
			m_lastSpawn = 0.0f;
			for (int i = 0; i < OBJECT_NR_1; i++)
			{
				if (!m_cars1[i]->isDrawn())
				{
					m_cars1[i]->setDrawn(true);
					break;
				}
			}
			m_drawnCars++;
		}
	}

	// Transformation logic
	for (int i = 0; i < m_cars1.size(); i++)
	{
		if (m_cars1[i]->isDrawn()) // If car has spawned
		{
			if (m_cars1[i]->animate(dt))
			{
				m_cars1[i]->setDrawn(false); // Hide Car
				m_drawnCars--;
			}
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
