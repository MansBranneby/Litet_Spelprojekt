#pragma once
#include "BoundingVolume.h"
#include "OBB.h"

class BoundingSphere : public BoundingVolume
{
private:
	float m_radius;

	// Help functions
	DirectX::XMVECTOR getClosestPointFromPointToOBB(OBB* other);
	CollisionInfo intersectsWithOBB(BoundingVolume* other);
	CollisionInfo intersectsWithSphere(BoundingVolume* other);

public:
	BoundingSphere();
	BoundingSphere(DirectX::XMVECTOR pos, float radius);
	~BoundingSphere();
	void update(DirectX::XMMATRIX modelMatrix);


	float getRadius() const;

	CollisionInfo intersects(BoundingVolume* other);
	CollisionInfo intersectsWithTriangle(DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c);
};


