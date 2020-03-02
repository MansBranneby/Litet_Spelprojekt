#include "DBOHandler.h"

DBOHandler::DBOHandler()
{
	// ANIMATION 1 (Freeway, left)
	for (int i = 0; i < OBJECT_NR_1 / 2; i++)
		m_cars1.push_back(new DBGObj(Animation::e_FreewayL, false, (float)(rand() % 20) + 50.0f));
	// ANIMATION 2 (Freeway, Right)
	for (int i = OBJECT_NR_1 / 2; i < OBJECT_NR_1; i++)
		m_cars1.push_back(new DBGObj(Animation::e_FreewayR, false, (float)(rand() % 20) + 50.0f));

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
	if (m_lastSpawn >= 1.0f && m_bigTimer >= 0.5f)
	{
		m_bigTimer = 0.0f;
		int random = rand() % 10;
		int random2 = rand() % 2;
		int index = 0;
		if (random2 > 0.5f)
			index = 5;
		if (random + m_lastSpawn >= 9.0f && m_drawnCars < OBJECT_NR_1)
		{
			m_lastSpawn = 0.0f;
			for (int i = index; i < OBJECT_NR_1; i++)
			{
				if (!m_cars1[i]->isDrawn())
				{
					m_cars1[i]->setDrawn(true);
					m_drawnCars++;
					break;
				}
			}
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

bool DBOHandler::isDrawn(int index)
{
	return m_cars1[index]->isDrawn();
}

//void DBOHandler::draw()
//{
//}
