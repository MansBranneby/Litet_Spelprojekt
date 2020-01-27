#pragma once
#include <DirectXMath.h>
#include <vector>

// Information acquired from collision detection
struct CollisionInfo
{
	bool m_colliding;
	DirectX::XMFLOAT2 m_normal;

	CollisionInfo()
	{
		m_colliding = false;
		m_normal = DirectX::XMFLOAT2(0.0f, 0.0f);
	}
};

class BoundingVolume
{
private:
	DirectX::XMFLOAT2 m_pos;

	virtual CollisionInfo intersectsWithPlane(BoundingVolume* other) = 0;
	virtual CollisionInfo intersectsWithOBB(BoundingVolume* other) = 0;

public:
	BoundingVolume();
	BoundingVolume(DirectX::XMFLOAT2 pos);
	virtual ~BoundingVolume();
	
	DirectX::XMFLOAT2 getPos();

	virtual CollisionInfo intersects(BoundingVolume* other) = 0;
};
