#pragma once
#include <DirectXMath.h>
#include "StructsAndEnums.h"
#include <vector>

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
