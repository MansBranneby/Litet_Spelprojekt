#pragma once
#include <DirectXMath.h>
#include <vector>

// Information acquired from collision detection
struct CollisionInfo
{
	bool m_colliding;
	DirectX::XMFLOAT3 m_normal;

	CollisionInfo()
	{
		m_colliding = false;
		m_normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
};

class BoundingVolume
{
private:
	DirectX::XMFLOAT3 m_pos;
	
	virtual CollisionInfo intersectsWithOBB(BoundingVolume* other) = 0;
	virtual CollisionInfo intersectsWithSphere(BoundingVolume* other) = 0;
public:
	BoundingVolume();
	BoundingVolume(DirectX::XMFLOAT3 pos);
	virtual ~BoundingVolume();
	
	DirectX::XMFLOAT3 getPos();

	virtual CollisionInfo intersects(BoundingVolume* other) = 0;
};
