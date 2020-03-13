#pragma once
#include <vector>
#include <DirectXMath.h>
#include "CollisionTests.h"
#include "PreLoader.h"

using namespace DirectX;

class QuadtreeNode
{
private:
	std::vector <QuadtreeNode*> m_children;
	std::vector <DirectX::XMFLOAT3> m_cMeshes;

	// Bounding volume
	boundingData m_boundingData;

	std::vector<DirectX::XMFLOAT3> calculateNewNodePositions(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD);

public:
	QuadtreeNode();
	QuadtreeNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, PreLoader* preLoader, unsigned int levels, unsigned int currentLevel);
	~QuadtreeNode();

	CollisionInfo testCollision(boundingData boundingVolume);
	CollisionInfo testCollision(boundingData boundingVolume, DirectX::XMVECTOR previousPos);
	float testCollisionT(XMFLOAT2 start, XMFLOAT2 end, float height = 1.5f);
	bool testCollision(XMFLOAT2 start, XMFLOAT2 end, float height = 0.5f);
	float testCollisionRay(XMFLOAT2 start, XMFLOAT2 dir);
};