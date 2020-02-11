#pragma once
#include <DirectXMath.h>
#include "StructsAndEnums.h"
#include <vector>
#include "Model.h"

// Information acquired from collision detection
struct CollisionInfo
{
	bool m_colliding;
	DirectX::XMVECTOR m_normal;

	CollisionInfo()
	{
		m_colliding = false;
		m_normal = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f };
	}
};

class BoundingVolume
{
private:
	DirectX::XMFLOAT3 m_pos;
	virtual CollisionInfo intersectsWithTriangle(DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c) = 0;
	virtual CollisionInfo intersectsWithOBB(BoundingVolume* other) = 0;
	virtual CollisionInfo intersectsWithSphere(BoundingVolume* other) = 0;
public:
	BoundingVolume();
	BoundingVolume(DirectX::XMFLOAT3 pos);
	virtual ~BoundingVolume();
	
	DirectX::XMFLOAT3 getPos();

	virtual CollisionInfo intersects(BoundingVolume* other) = 0;
	DirectX::XMVECTOR getClosestPointFromPointToTriangle(DirectX::XMVECTOR p, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c);
};
