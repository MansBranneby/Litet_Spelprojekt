#pragma once
#include "Billboard.h"
#include "PreLoader.h"

class BillboardHandler
{
private:
	std::vector <Billboard> m_billboards;
	std::vector <Billboard> m_numberBillboards;
	std::vector <Billboard> m_staticBillboards;
	std::vector <Billboard> m_staticBillboardPlatform;

public:
	BillboardHandler();
	BillboardHandler(PreLoader* preLoader, std::vector<objectType> objectTypes);
	~BillboardHandler();

	std::vector <Billboard> getBillboards() const;
	std::vector <Billboard> getBillboardsOfType(objectType objectType) const;
	int getNrOfBillboards() const;
	BillboardData getBillboardData(int i) const;

	void updateBillboards(float dt);
	void setAllStates(int i, float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV);
	void setNoneState(int i);
};