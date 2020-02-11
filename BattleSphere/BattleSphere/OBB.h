#pragma once
#include "BoundingVolume.h"

class OBB : public BoundingVolume
{
private:
	// Local axes
	DirectX::XMVECTOR m_xAxis;
	DirectX::XMVECTOR m_zAxis;
	std::vector<DirectX::XMVECTOR> m_axes;
	std::vector<float> m_halfWidthDepth;
	//TODO: FIXME

	// half width and depth
	DirectX::XMFLOAT2 m_halfWD;

	// Help functions
	DirectX::XMVECTOR getClosestPointFromPointToOBB(DirectX::XMVECTOR p);

	// intersection tests for basic primitives
	CollisionInfo intersectsWithTriangle(DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c);
	CollisionInfo intersectsWithOBB(BoundingVolume* other);
	CollisionInfo intersectsWithSphere(BoundingVolume* other);

public:
	OBB();
	OBB(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, XMVECTOR xAxis, XMVECTOR zAxis);
	OBB(boundingData data);
	~OBB();

	DirectX::XMFLOAT2 getHalfWD() const;
	std::vector<DirectX::XMVECTOR> getAxes() const;

	CollisionInfo intersects(BoundingVolume* other);
};