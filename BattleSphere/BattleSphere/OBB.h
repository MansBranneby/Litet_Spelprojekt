#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	// Local axes
	DirectX::XMVECTOR m_xAxis;
	DirectX::XMVECTOR m_zAxis;
	std::vector<DirectX::XMVECTOR> m_axes;

	// half width and height
	DirectX::XMFLOAT2 m_halfWD;

	CollisionInfo intersectsWithOBB(BoundingVolume* other);

public:
	OBB();
	OBB(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 halfWD, DirectX::XMMATRIX rotationMatrix);
	~OBB();

	DirectX::XMFLOAT2 getHalfWD();
	std::vector<DirectX::XMVECTOR> getAxes();

	CollisionInfo intersects(BoundingVolume* other);
};