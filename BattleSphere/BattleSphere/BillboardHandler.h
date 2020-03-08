#pragma once
#include "Billboard.h"
#include "PreLoader.h"

class BillboardHandler
{
private:
	std::vector <Billboard> m_billboards;

public:
	BillboardHandler();
	BillboardHandler(PreLoader* preLoader, std::vector<ObjectType> objectTypes);
	~BillboardHandler();

	std::vector <Billboard> getBillboards() const;
	int getNrOfBillboards() const;
	BillboardData getBillboardData(int i) const;
	void updateBillboards(float dt);
	void setAllStates(int i, float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV);
	void setNoneState(int i);
};