#include "BoundingVolume.h"

//CollisionInfo BoundingVolume::intersectsWithTriangle()
//{
//	CollisionInfo collisionInfo;
//
//
//
//	return collisionInfo;
//}

DirectX::XMVECTOR BoundingVolume::getClosestPointFromPointToTriangle(DirectX::XMVECTOR p, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c)
{
	DirectX::XMVECTOR ab = b - a;
	DirectX::XMVECTOR ac = c - a;
	DirectX::XMVECTOR ap = p - a;
	
	// Check if p outside a
	float d1 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(ab, ap));
	float d2 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(ac, ap));

	if (d1 <= 0.0f && d2 <= 0.0f)
		return a;

	DirectX::XMVECTOR bp = p - b;
	
	// Check if p outside b
	float d3 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(ab, bp));
	float d4 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(ac, bp));

	if (d3 >= 0.0f && d4 <= d3)
		return b;

	// Check if p outside ab
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return a + v * ab;
	}

	DirectX::XMVECTOR cp = p - c;
	// Check if p outside c
	float d5 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(ab, cp));
	float d6 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(ac, cp));

	if (d6 >= 0.0f && d5 <= d6)
		return c;

	// Check if p outside ac
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return a + w * ac;
	}

	// Check if p outside bc
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b);
	}

	// p inside face region. Compute q through its barycentric coordinates (u, v, w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w; 
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


