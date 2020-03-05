#pragma once
#include "Billboard.h"
#include "PreLoader.h"

class BillboardHandler
{
private:
	std::vector <Billboard> m_billboards;

public:
	BillboardHandler();
	BillboardHandler(PreLoader* preLoader);
	~BillboardHandler();

	std::vector <Billboard> getBillboards() const; 
	int getNrOfBillboards() const;
	BillboardData getBillboardData(int i) const;
	void updateBillboards(float dt);

};