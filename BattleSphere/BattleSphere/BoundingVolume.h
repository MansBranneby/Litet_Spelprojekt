#pragma once
#include <DirectXMath.h>
#include "StructsAndEnums.h"
#include <vector>

class BoundingVolume
{
private:
	DirectX::XMVECTOR m_pos;
	virtual CollisionInfo intersectsWithOBB(BoundingVolume* other) = 0;
	virtual CollisionInfo intersectsWithSphere(BoundingVolume* other) = 0;
public:
	BoundingVolume();
	BoundingVolume(DirectX::XMVECTOR pos);
	virtual ~BoundingVolume();
	
	DirectX::XMVECTOR getPos() const;
	void setPos(DirectX::XMVECTOR pos);
	virtual void update(DirectX::XMMATRIX modelMatrix) = 0;

	virtual CollisionInfo intersects(BoundingVolume* other) = 0;
	virtual CollisionInfo intersectsWithTriangle(DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c) = 0;
	DirectX::XMVECTOR getClosestPointFromPointToTriangle(DirectX::XMVECTOR p, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c);


};
