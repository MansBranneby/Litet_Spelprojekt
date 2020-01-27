#include "OBB.h"
//#include <stdlib.h> 
	
CollisionInfo OBB::intersectsWithOBB(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	// TODO B to A's space

	// Cast bounding volume to OBB
	other = static_cast<OBB*> (other);

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat2(&getPos());
	DirectX::XMVECTOR otherPos = DirectX::XMLoadFloat2(&other->getPos());
	DirectX::XMVECTOR vecToOBB = DirectX::XMVectorSubtract(otherPos, pos);

	// SA stands for seperating axis
	// SA: Ax
	float projection = abs(DirectX::XMVectorGetX(DirectX::XMVector2Dot(vecToOBB, m_xAxis)));
	//float proj = 

	//return CollisionInfo();
}

OBB::OBB()
{
	// Local axes
	DirectX::XMVECTOR m_xAxis = {1.0f, 0.0};
	DirectX::XMVECTOR m_zAxis = {0.0, 1.0};

	// half width and height
	DirectX::XMFLOAT2 m_halfWD = { 0.0f, 0.0f };
}

OBB::OBB(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 widthDepth, DirectX::XMMATRIX rotationMatrix)
	:BoundingVolume(pos)
{
	// Calculate half width and depth
	m_halfWD.x = pos.x - widthDepth.x;
	m_halfWD.y = pos.y - widthDepth.y;

	// Define axes
	m_xAxis = { 1.0f, 0.0 };
	m_zAxis = { 0.0f, 1.0 };

	// Transform axes
	m_xAxis = DirectX::XMVector2Transform(m_xAxis, rotationMatrix);
	m_zAxis = DirectX::XMVector2Transform(m_zAxis, rotationMatrix);

	// Push into vector
	m_axes = { m_xAxis, m_zAxis };
}

OBB::~OBB()
{
}

DirectX::XMFLOAT2 OBB::getHalfWD()
{
	return m_halfWD;
}

CollisionInfo OBB::intersects(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	if (dynamic_cast<OBB*> (other))
		collisionInfo = intersectsWithOBB(other);

	return collisionInfo;
}
