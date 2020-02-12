#include "OBB.h"
#include "BoundingSphere.h"
	
DirectX::XMVECTOR OBB::getClosestPointFromPointToOBB(DirectX::XMVECTOR p)
{
	// vector from point to OBB
	DirectX::XMVECTOR d = getPos() - p;

	// Closest point starts at OBB center and makes steps from there
	DirectX::XMVECTOR closestPoint = getPos();

	for (int i = 0; i < m_axes.size(); ++i)
	{
		// Project vector from point to OBB onto axes
		float t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, m_axes[i]));
		// If projection is longer than axis then clamp it that axis' length
		if (t >= m_halfWidthDepth[i]) t = m_halfWidthDepth[i];
		else if (t <= -m_halfWidthDepth[i]) t = -m_halfWidthDepth[i];
		// Step along projected axis
		closestPoint += t * m_axes[i];
	}

	return closestPoint;
}

CollisionInfo OBB::intersectsWithTriangle(DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c)
{
	return CollisionInfo();
}

CollisionInfo OBB::intersectsWithOBB(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	// Cast bounding volume to OBB
	OBB* otherOBB = static_cast<OBB*> (other);

	DirectX::XMVECTOR pos = getPos();
	DirectX::XMVECTOR otherPos = other->getPos();
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
	BoundingSphere* sphere = static_cast<BoundingSphere*> (other);
	DirectX::XMVECTOR closestPoint = getClosestPointFromPointToOBB(sphere->getPos());
	DirectX::XMVECTOR d = closestPoint - sphere->getPos();
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(d));

	if (dist < sphere->getRadius())
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_normal = d;
	}

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

OBB::OBB(DirectX::XMVECTOR pos, DirectX::XMFLOAT2 halfWD, XMVECTOR xAxis, XMVECTOR zAxis)
	:BoundingVolume(pos)
{
	// Calculate half width and depth
	m_halfWD.x = halfWD.x;
	m_halfWD.y = halfWD.y;
	m_halfWidthDepth = { halfWD.x, halfWD.y };

	// Define axes
	m_xAxis = xAxis;
	m_zAxis = zAxis;

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

void OBB::update(DirectX::XMMATRIX modelMatrix)
{
	setPos(DirectX::XMVector4Transform(getPos(), modelMatrix));
}

DirectX::XMFLOAT2 OBB::getHalfWD() const
{
	return m_halfWD;
}

std::vector<float> OBB::getHalfWDVector() const
{
	return m_halfWidthDepth;
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
	if (dynamic_cast<BoundingSphere*> (other))
		collisionInfo = intersectsWithSphere(other);

	return collisionInfo;
}
