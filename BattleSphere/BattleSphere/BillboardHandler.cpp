#include "BillboardHandler.h"

BillboardHandler::BillboardHandler()
{
	m_billboards.push_back(Billboard({ -0.05f, 0.0f, 0.0f }, 0.1f, 0.1f, 1.0f, 0, 0, 0));
}

BillboardHandler::BillboardHandler(PreLoader* preLoader)
{
	// Create as many billboards as submodels
	for (int i = 0; i < preLoader->getNrOfVariants(ObjectType::e_billboard); ++i)
	{
		for (int j = 0; j < preLoader->getNrOfModels(ObjectType::e_billboard, i); ++j)
		{
			for (int k = 0; k < preLoader->getNrOfSubModels(ObjectType::e_billboard, j, i); ++k)
				m_billboards.push_back(Billboard(i, j, k));
		}
	}

	//if (k == 2)
	//	m_billboards.push_back(Billboard({ -0.05f, 0.0f }, 0.0f, 0.1f, 1, i, j, k));
	//else if (k == 3)
	//	m_billboards.push_back(Billboard({ 0.0f, 0.0f }, 0.0f, 0.1f, 1, i, j, k));
	//else
}

BillboardHandler::~BillboardHandler()
{
}

std::vector<Billboard> BillboardHandler::getBillboards() const
{
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
