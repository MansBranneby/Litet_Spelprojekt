#include "BillboardHandler.h"

BillboardHandler::BillboardHandler()
{
}

BillboardHandler::BillboardHandler(PreLoader* preLoader)
{
	// Create as many billboards as submodels for all e_billboards
	for (int i = 0; i < preLoader->getNrOfVariants(ObjectType::e_billboard); ++i)
	{
		for (int j = 0; j < preLoader->getNrOfModels(ObjectType::e_billboard, i); ++j)
		{
			for (int k = 0; k < preLoader->getNrOfSubModels(ObjectType::e_billboard, j, i); ++k)
			{
				BillboardData tempBillboardData = preLoader->getSubModelBillboardData(ObjectType::e_billboard, i, j, k);
				m_billboards.push_back(Billboard(i, j, k, tempBillboardData)); // Each billboard knows its variantNr, modelNr and submodelNr
			}
		}
	}

	// Create as many billboards as submodels for all static e_static_billboards
	for (int i = 0; i < preLoader->getNrOfVariants(ObjectType::e_static_billboard); ++i)
	{
		for (int j = 0; j < preLoader->getNrOfModels(ObjectType::e_static_billboard, i); ++j)
		{
			for (int k = 0; k < preLoader->getNrOfSubModels(ObjectType::e_static_billboard, j, i); ++k)
			{
				BillboardData tempBillboardData = preLoader->getSubModelBillboardData(ObjectType::e_static_billboard, i, j, k);
				m_staticBillboards.push_back(Billboard(i, j, k, tempBillboardData)); // Each billboard knows its variantNr, modelNr and submodelNr
			}
		}
	}
	//// predefined colours
	//DirectX::XMVECTOR cyan = { 0.0f, 0.4f, 0.3f, 1.0f };
	//DirectX::XMVECTOR lightblue = {0.0f, 0.3f, 0.4f, 1.0f };
	//DirectX::XMVECTOR pink = { 1.0f, 0.01f, 0.6f , 1.0f};
	//DirectX::XMVECTOR lightPink = { 0.6f, 0.2f, 0.5f , 1.0f };
	//DirectX::XMVECTOR purple = { 0.6f, 0.0f, 0.8f , 1.0f };
	//DirectX::XMVECTOR darkPurple = { 0.4f, 0.0f, 0.4f , 1.0f };
}

BillboardHandler::~BillboardHandler()
{
}

std::vector<Billboard> BillboardHandler::getBillboards() const
{
	return m_billboards;
}

std::vector<Billboard> BillboardHandler::getStaticBillboards() const
{
	return m_staticBillboards;
}

int BillboardHandler::getNrOfBillboards() const
{
	return (int)m_billboards.size();
}

int BillboardHandler::getNrOfStaticBillboards() const
{
	return (int)m_staticBillboards.size();
}

BillboardData BillboardHandler::getBillboardData(int i) const
{
	// Get billboardData for specific billboard
	return m_billboards[i].getBillboardData();
}

BillboardData BillboardHandler::getStaticBillboardData(int i) const
{
	return m_staticBillboards[i].getBillboardData();
}

void BillboardHandler::updateBillboards(float dt)
{
	// Update billboards
	for (int i = 0; i < m_billboards.size(); ++i)
		m_billboards[i].update(dt);
}
