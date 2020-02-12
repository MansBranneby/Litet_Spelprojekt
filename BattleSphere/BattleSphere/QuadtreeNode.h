#pragma once
#include <vector>
#include <DirectXMath.h>
#include "GameObject.h"
#include "PreLoader.h"
#include "BoundingVolume.h"
#include "OBB.h"

class QuadtreeNode
{
private:
	std::vector <QuadtreeNode*> m_children;
	std::vector <DirectX::XMFLOAT3> m_cMeshes;

	// Bounding volume
	BoundingVolume* m_boundingVolume;

	std::vector<DirectX::XMFLOAT3> calculateNewNodePositions(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD);

public:
	QuadtreeNode();
	//	Dimensions of box
	QuadtreeNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, PreLoader preLoader, unsigned int levels, unsigned int currentLevel);
	~QuadtreeNode();
};