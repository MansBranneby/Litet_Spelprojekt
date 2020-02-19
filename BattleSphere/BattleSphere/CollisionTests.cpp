#pragma once
#include "StructsAndEnums.h"
#include "CollisionTests.h"

DirectX::XMVECTOR getClosestPointFromPointToTriangle(DirectX::XMVECTOR p, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c)
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

CollisionInfo testSphereTriangle(DirectX::XMVECTOR SpherePos, float sphereRad, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c)
{
	CollisionInfo collisionInfo;
	DirectX::XMVECTOR closestPoint = getClosestPointFromPointToTriangle(SpherePos, a, b, c);
	DirectX::XMVECTOR d = SpherePos - closestPoint;
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(d));
	if (dist <= sphereRad)
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_contactPoint = closestPoint;
		DirectX::XMVECTOR triNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(b - a, c - a));

		// Signed distance between triangle and sphere
		float signedDist = DirectX::XMVectorGetX(DirectX::XMVector3Dot(triNormal, d));
		if (signedDist < 0.0f)
			collisionInfo.m_normal = triNormal * (sphereRad + dist);
		else
			collisionInfo.m_normal = triNormal * (sphereRad - dist);
	}

	return collisionInfo;
}

DirectX::XMVECTOR getClosestPointFromPointToOBB(DirectX::XMVECTOR point, boundingData OBB)
{
	// vector from point to OBB
	DirectX::XMVECTOR d = point - OBB.pos;

	// Closest point starts at OBB center and makes steps from there
	DirectX::XMVECTOR closestPoint = OBB.pos;

	// Project vector from point to OBB onto axes
	float t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, OBB.xAxis));
	// If projection is longer than axis then clamp it that axis' length
	if (t >= OBB.halfWD.x) t = OBB.halfWD.x;
	else if (t <= -OBB.halfWD.x) t = -OBB.halfWD.x;
	// Step along projected axis
	closestPoint += t * OBB.xAxis;

	// Project vector from point to OBB onto axes
	t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(d, OBB.zAxis));
	// If projection is longer than axis then clamp it that axis' length
	if (t >= OBB.halfWD.y) t = OBB.halfWD.y;
	else if (t <= -OBB.halfWD.y) t = -OBB.halfWD.y;
	// Step along projected axis
	closestPoint += t * OBB.zAxis;

	return closestPoint;
}

CollisionInfo testOBBSphere(DirectX::XMVECTOR spherePos, float sphereRad, boundingData OBB)
{
	CollisionInfo collisionInfo;

	// Set sphere y to OBB y because this is a 2D collision test
	spherePos = DirectX::XMVectorSetY(spherePos, DirectX::XMVectorGetY(OBB.pos));
	DirectX::XMVECTOR d = getClosestPointFromPointToOBB(spherePos, OBB) - spherePos;

	if (DirectX::XMVectorGetX(DirectX::XMVector3Length(d)) < sphereRad)
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_normal = d;
	}

	return collisionInfo;
}

CollisionInfo testOBBOBB(boundingData aOBB, boundingData bOBB)
{
	CollisionInfo collisionInfo;

	DirectX::XMVECTOR vecToOBB = DirectX::XMVectorSubtract(bOBB.pos, aOBB.pos);

	// SA stands for seperating axis
	// SA: Ax
	float projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, aOBB.xAxis)));
	float proj = aOBB.halfWD.x
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(bOBB.xAxis, bOBB.halfWD.x), aOBB.xAxis)))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(bOBB.zAxis, bOBB.halfWD.y), aOBB.xAxis)));
	if (projection > proj)
		return collisionInfo;

	// SA: Az
	projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, aOBB.zAxis)));
	proj = aOBB.halfWD.y
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(bOBB.xAxis, bOBB.halfWD.x), aOBB.zAxis)))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(bOBB.zAxis, bOBB.halfWD.y), aOBB.zAxis)));
	if (projection > proj)
		return collisionInfo;

	// SA: Bx
	projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, bOBB.xAxis)));
	proj = bOBB.halfWD.x
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(aOBB.xAxis, aOBB.halfWD.x), bOBB.xAxis)))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(aOBB.zAxis, aOBB.halfWD.y), bOBB.xAxis)));
	if (projection > proj)
		return collisionInfo;

	// SA: By
	projection = abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vecToOBB, bOBB.zAxis)));
	proj = bOBB.halfWD.y
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(aOBB.xAxis, aOBB.halfWD.x), bOBB.zAxis)))
		+ abs(DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(aOBB.zAxis, aOBB.halfWD.y), bOBB.zAxis)));
	if (projection > proj)
		return collisionInfo;

	// If we're here there is a collision
	collisionInfo.m_colliding = true;

	return collisionInfo;
}

CollisionInfo testSphereSphere(DirectX::XMVECTOR aPos, DirectX::XMVECTOR bPos, float aRad, float bRad)
{
	CollisionInfo collisionInfo;

	DirectX::XMVECTOR sphereToOther = DirectX::XMVectorSubtract(aPos, bPos);
	float dSphere = DirectX::XMVectorGetX(DirectX::XMVector3Length(sphereToOther));

	if (dSphere < aRad + bRad)
	{
		collisionInfo.m_colliding = true;
		collisionInfo.m_normal = sphereToOther;
	}

	return collisionInfo;
}

CollisionInfo testMovingSphereSphere(DirectX::XMVECTOR aPos, DirectX::XMVECTOR bPos, float aRad, float bRad, DirectX::XMVECTOR aVel, DirectX::XMVECTOR bVel)
{
	return CollisionInfo();
}
