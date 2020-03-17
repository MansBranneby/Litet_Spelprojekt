#include "BillboardHandler.h"

BillboardHandler::BillboardHandler()
{
}

BillboardHandler::BillboardHandler(PreLoader* preLoader, std::vector<objectType> objectTypes)
{
	for(int i = 0; i < objectTypes.size(); ++i)
	{
		// Create as many billboards as submodels for all the types in inside "createTheseBillboards"
		for (int j = 0; j < preLoader->getNrOfVariants(objectTypes[i]); ++j)
		{
			for (int k = 0; k < preLoader->getNrOfModels(objectTypes[i], j); ++k)
			{
				for (int l = 0; l < preLoader->getNrOfSubModels(objectTypes[i], j, k); ++l)
				{
					BillboardData tempBillboardData = preLoader->getSubModelBillboardData(objectTypes[i], j, k, l);
					m_billboards.push_back(Billboard(objectTypes[i], j, k, l, tempBillboardData)); // Each billboard knows its variantNr, modelNr and submodelNr
				}
			}
		}
	}

	for (int i = 0; i < m_billboards.size(); ++i)
	{
		switch (m_billboards[i].getObjectType())
		{
			case objectType::e_number_billboard:
				m_numberBillboards.push_back(m_billboards[i]);
				std::rotate(m_numberBillboards.rbegin(), m_numberBillboards.rbegin() + 1, m_numberBillboards.rend());
				break;
			case objectType::e_ranking_billboard:
				m_rankingBillboards.push_back(m_billboards[i]);
				std::rotate(m_rankingBillboards.rbegin(), m_rankingBillboards.rbegin() + 1, m_rankingBillboards.rend());
				break;
			case objectType::e_static_billboard:
				m_staticBillboards.push_back(m_billboards[i]);
				break;
			case objectType::e_static_billboard_score_platform:
				m_staticBillboardPlatform.push_back(m_billboards[i]);
				break;
			case objectType::e_platformbar_billboard:
				m_platformBarsBillboards.push_back(m_billboards[i]);
				break;
			case objectType::e_static_billboard_score:
				m_staticBillboardScoreRoom.push_back(m_billboards[i]);
				break;
		}
	}
}

BillboardHandler::~BillboardHandler()
{
}

std::vector<Billboard> BillboardHandler::getBillboards() const
{
	return m_billboards;
}

std::vector<Billboard> BillboardHandler::getBillboardsOfType(objectType objectType) const
{
	switch (objectType)
	{
	case objectType::e_number_billboard:
		return m_numberBillboards;
		break;
	case objectType::e_static_billboard:
		return m_staticBillboards;
		break;
	case objectType::e_static_billboard_score_platform:
		return m_staticBillboardPlatform;
		break;
	case objectType::e_platformbar_billboard:
		return m_platformBarsBillboards;
		break;
	case objectType::e_static_billboard_score:
		return m_staticBillboardScoreRoom;
		break;
	}

	return m_billboards;
}

int BillboardHandler::getNrOfBillboards() const
{
	return (int)m_billboards.size();
}

BillboardData BillboardHandler::getBillboardData(int i) const
{
	// Get billboardData for specific billboard
	return m_billboards[i].getBillboardData();
}

void BillboardHandler::updateBillboards(float dt)
{
	// Update billboards
	for (int i = 0; i < m_billboards.size(); ++i)
		m_billboards[i].update(dt);
}

void BillboardHandler::setAllStates(int i,float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV)
{
	m_billboards[i].setAllStates(flashSpeed, colourChangeSpeed, colourA, colourB, velocityUV);	
}

void BillboardHandler::setNoneState(int i)
{
	m_billboards[i].setState(BillboardState::e_none);
}
