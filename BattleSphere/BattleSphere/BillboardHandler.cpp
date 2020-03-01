#include "BillboardHandler.h"

BillboardHandler::BillboardHandler()
{
}

BillboardHandler::BillboardHandler(PreLoader* preLoader)
{
	// Create as many billboards as submodels
	for (int i = 0; i < preLoader->getNrOfVariants(ObjectType::e_billboard); ++i)
	{
		for (int j = 0; j < preLoader->getNrOfModels(ObjectType::e_billboard, i); ++j)
		{
			for (int k = 0; k < preLoader->getNrOfSubModels(ObjectType::e_billboard, j, i); ++k)
				m_billboards.push_back(Billboard(i, j, k)); // Each billboard knows its variantNr, modelNr and submodelNr
		}
	}

	m_billboards[2].setType(BillboardState::e_all);
	m_billboards[5].setType(BillboardState::e_all);
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
