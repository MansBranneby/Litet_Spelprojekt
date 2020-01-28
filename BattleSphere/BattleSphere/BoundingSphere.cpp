#include "BoundingSphere.h"

CollisionInfo BoundingSphere::intersectsWithOBB(BoundingVolume* other)
{
	CollisionInfo collisionInfo;
	return collisionInfo;
}

CollisionInfo BoundingSphere::intersectsWithSphere(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	BoundingSphere* otherSphere = dynamic_cast <BoundingSphere*> (other);

	
	DirectX::XMVECTOR vSphere = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&getPos()), DirectX::XMLoadFloat3(&otherSphere->getPos()));
	float dSphere = DirectX::XMVectorGetX(DirectX::XMVector3Length(vSphere));

	if (m_radius + otherSphere->getRadius() > dSphere)
		collisionInfo.m_colliding = true;

	return collisionInfo;
}

BoundingSphere::BoundingSphere()
{
	m_radius = 0.0f;
}

BoundingSphere::BoundingSphere(DirectX::XMFLOAT3 pos, float radius)
	:BoundingVolume(pos)
{
	m_radius = radius;
}

BoundingSphere::~BoundingSphere()
{
}

float BoundingSphere::getRadius() const
{
	return m_radius;
}

CollisionInfo BoundingSphere::intersects(BoundingVolume* other)
{	
	CollisionInfo collisionInfo;

	if (dynamic_cast<OBB*> (other))
		collisionInfo = intersectsWithOBB(other);
	if (dynamic_cast<BoundingSphere*> (other))
		collisionInfo = intersectsWithSphere(other);

	return collisionInfo;
}
