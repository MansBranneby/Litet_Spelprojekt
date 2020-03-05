#pragma once
#include "Billboard.h"
#include "PreLoader.h"

class BillboardHandler
{
private:
	std::vector <Billboard> m_billboards;
	std::vector <Billboard> m_staticBillboards;

public:
	BillboardHandler();
	BillboardHandler(PreLoader* preLoader);
	~BillboardHandler();

	std::vector <Billboard> getBillboards() const;
	std::vector <Billboard> getStaticBillboards() const;
	int getNrOfBillboards() const;
	int getNrOfStaticBillboards() const;
	BillboardData getBillboardData(int i) const;
	BillboardData getStaticBillboardData(int i) const;
	void updateBillboards(float dt);

};