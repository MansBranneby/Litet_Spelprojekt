#pragma once
#include "BoundingVolume.h"
#include "OBB.h"

class BoundingSphere : public BoundingVolume
{
private:
	float m_radius;
	CollisionInfo intersectsWithOBB(BoundingVolume* other);
	CollisionInfo intersectsWithSphere(BoundingVolume* other);
public:
	BoundingSphere();
	BoundingSphere(DirectX::XMFLOAT3 pos, float radius);
	~BoundingSphere();
	float getRadius() const;

	CollisionInfo intersects(BoundingVolume* other);
};


