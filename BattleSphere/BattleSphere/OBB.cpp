#include "OBB.h"
//#include <stdlib.h> 
	
CollisionInfo OBB::intersectsWithOBB(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	// Cast bounding volume to OBB
	OBB* otherOBB = static_cast<OBB*> (other);

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&getPos());
	DirectX::XMVECTOR otherPos = DirectX::XMLoadFloat3(&other->getPos());
	DirectX::XMVECTOR vecToOBB = DirectX::XMVectorSubtract(otherPos, pos);
	
	// SA stands for seperating axis
	// SA: Ax
	float projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, m_xAxis)));
	float proj = m_halfWD.x
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(otherOBB->getAxes()[0], otherOBB->getHalfWD().x), m_xAxis)))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(otherOBB->getAxes()[1], otherOBB->getHalfWD().y), m_xAxis)));
	if (projection > proj)
		return collisionInfo;
		
	// SA: Az
	projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, m_zAxis)));
	proj = m_halfWD.y
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(otherOBB->getAxes()[0], otherOBB->getHalfWD().x), m_zAxis)))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(otherOBB->getAxes()[1], otherOBB->getHalfWD().y), m_zAxis)));
	if (projection > proj)
		return collisionInfo;

	// SA: Bx
	projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, otherOBB->getAxes()[0])));
	proj = otherOBB->getHalfWD().x 
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(m_xAxis, m_halfWD.x), otherOBB->getAxes()[0])))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(m_zAxis, m_halfWD.y), otherOBB->getAxes()[0])));
	if(projection > proj)
		return collisionInfo;

	// SA: By
	projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, otherOBB->getAxes()[1])));
	proj = otherOBB->getHalfWD().y 
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(m_xAxis, m_halfWD.x), otherOBB->getAxes()[1])))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(m_zAxis, m_halfWD.y), otherOBB->getAxes()[1])));
	if (projection > proj)
		return collisionInfo;

	// If we're here there is a collision
	collisionInfo.m_colliding = true;

	return collisionInfo;
}

CollisionInfo OBB::intersectsWithSphere(BoundingVolume* other)
{
	CollisionInfo collisionInfo;
	return collisionInfo;
}

OBB::OBB()
{
	// Local axes
	m_xAxis = {1.0f, 0.0};
	m_zAxis = {0.0, 1.0};

	// half width and height
	m_halfWD = { 0.0f, 0.0f };
}

OBB::OBB(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, DirectX::XMMATRIX rotationMatrix)
	:BoundingVolume(pos)
{
	// Calculate half width and depth
	m_halfWD.x = halfWD.x;
	m_halfWD.y = halfWD.y;

	// Define axes
	m_xAxis = { 1.0f, 0.0f, 0.0f };
	m_zAxis = { 0.0f, 0.0f, 1.0f };

	// Transform axes
	m_xAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(m_xAxis, rotationMatrix));
	m_zAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(m_zAxis, rotationMatrix));

	// Push into vector
	m_axes = { m_xAxis, m_zAxis };
}

OBB::OBB(boundingData data)
	:BoundingVolume(data.pos)
{
	m_halfWD.x = data.halfWD.x;
	m_halfWD.y = data.halfWD.y;
	m_xAxis = data.xAxis;
	m_zAxis = data.zAxis;
}

OBB::~OBB()
{
}

DirectX::XMFLOAT2 OBB::getHalfWD() const
{
	return m_halfWD;
}

std::vector<DirectX::XMVECTOR> OBB::getAxes() const
{
	return m_axes;
}

CollisionInfo OBB::intersects(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	if (dynamic_cast<OBB*> (other))
		collisionInfo = intersectsWithOBB(other);

	return collisionInfo;
}
