#include "BillboardHandler.h"

BillboardHandler::BillboardHandler()
{
}

BillboardHandler::BillboardHandler(PreLoader* preLoader)
{
	// Create as many billboards as submodels
	for (int i = 0; i < preLoader->getNrOfVariants(objectType::e_billboard); ++i)
	{
		for (int j = 0; j < preLoader->getNrOfModels(objectType::e_billboard, i); ++j)
		{
			if (j == 1)
				float b = 10.0f;

			for (int k = 0; k < preLoader->getNrOfSubModels(objectType::e_billboard, j, i); ++k)
			{
				BillboardData tempBillboardData = preLoader->getSubModelBillboardData(objectType::e_billboard, i, j, k);
				m_billboards.push_back(Billboard(i, j, k, tempBillboardData)); // Each billboard knows its variantNr, modelNr and submodelNr
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

	//m_billboards[3].setAllStates(0.1f, 0.02f, lightPink, cyan, { -0.03f });
	//m_billboards[2].setAllStates(0.01f, 0.02f, darkPurple, cyan, {-0.03f});
	//m_billboards[5].setFlashState(0.05f);
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
