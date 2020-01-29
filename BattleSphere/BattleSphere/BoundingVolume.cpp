#include "BoundingVolume.h"

CollisionInfo BoundingVolume::intersectsWithTriangle()
{
	CollisionInfo collisionInfo;



	return collisionInfo;
}

BoundingVolume::BoundingVolume()
{
	m_pos = { 0.0f, 0.0f, 0.0f};
}

BoundingVolume::BoundingVolume(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

BoundingVolume::~BoundingVolume()
{
}

DirectX::XMFLOAT3 BoundingVolume::getPos()
{
	return m_pos;
}


