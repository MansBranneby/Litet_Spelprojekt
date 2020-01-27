#include "BoundingVolume.h"

BoundingVolume::BoundingVolume()
{
	m_pos = { 0.0f, 0.0f};
}

BoundingVolume::BoundingVolume(DirectX::XMFLOAT2 pos)
{
	m_pos = pos;
}

BoundingVolume::~BoundingVolume()
{
}

DirectX::XMFLOAT2 BoundingVolume::getPos()
{
	return m_pos;
}


