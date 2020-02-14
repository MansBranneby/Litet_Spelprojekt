#include "BoundingSphere.h"

CollisionInfo BoundingSphere::intersectsWithTriangle(DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c)
{
	CollisionInfo collisionInfo;
	XMVECTOR p = getPos();
	DirectX::XMVECTOR closestPoint = getClosestPointFromPointToTriangle(p, a, b, c);
	DirectX::XMVECTOR d = closestPoint - p;
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(closestPoint - p));
	if (dist <= m_radius)
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(b - a, c - a)) * (m_radius - dist);
	}

	return collisionInfo;

}

DirectX::XMVECTOR BoundingSphere::getClosestPointFromPointToOBB(OBB* other)
{
	OBB* obb = dynamic_cast<OBB*>(other);
	DirectX::XMVECTOR closestPoint{0.0f, 0.0f, 0.0f, 0.0f};
	if(obb)
	{ 
		// vector from point to OBB
		DirectX::XMVECTOR d = getPos() - obb->getPos();

		// Closest point starts at OBB center and makes steps from there
		closestPoint = obb->getPos();

		for (int i = 0; i < obb->getAxes().size(); ++i)
		{
			// Project vector from point to OBB onto axes
			float t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, obb->getAxes()[i]));
			// If projection is longer than axis then clamp it that axis' length
			if (t >= obb->getHalfWDVector()[i]) t = obb->getHalfWDVector()[i];
			else if (t <= -obb->getHalfWDVector()[i]) t = -obb->getHalfWDVector()[i];
			// Step along projected axis
			closestPoint += t * obb->getAxes()[i];
		}
	}

	return closestPoint;
}

CollisionInfo BoundingSphere::intersectsWithOBB(BoundingVolume* other)
{

	CollisionInfo collisionInfo;
	OBB* obb = static_cast<OBB*>(other);

	DirectX::XMVECTOR d = DirectX::XMVectorSetY(getPos(), 0.0f) - getClosestPointFromPointToOBB(obb);
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(d));

	if (dist < m_radius)
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_normal = DirectX::XMVector3Normalize(d) * (m_radius-dist);
	}

	return collisionInfo;
}

CollisionInfo BoundingSphere::intersectsWithSphere(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	BoundingSphere* otherSphere = dynamic_cast <BoundingSphere*> (other);

	
	DirectX::XMVECTOR sphereToOther = DirectX::XMVectorSubtract(getPos(), otherSphere->getPos());
	float dSphere = DirectX::XMVectorGetX(DirectX::XMVector3Length(sphereToOther));

	if (dSphere < m_radius + otherSphere->getRadius())
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_normal = sphereToOther;
	}

	return collisionInfo;
}

BoundingSphere::BoundingSphere()
{
	m_radius = 0.0f;
}

BoundingSphere::BoundingSphere(DirectX::XMVECTOR pos, float radius)
	:BoundingVolume(pos)
{
	m_radius = radius;
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::update(DirectX::XMMATRIX modelMatrix)
{
	setPos(DirectX::XMVector4Transform(getPos(), modelMatrix));
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
