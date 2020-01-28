#pragma once
#include <vector>
#include <DirectXMath.h>
#include "BoundingVolume.h"
#include "OBB.h"

class QuadtreeNode
{
private:
	std::vector <QuadtreeNode*> m_children;
	std::vector <BoundingVolume*> m_models;

	// Bounding volume
	BoundingVolume* m_boundingVolume;

	std::vector<DirectX::XMFLOAT2> calculateNewNodePositions(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 halfWD);

public:
	QuadtreeNode();
	//	Dimensions of box
	QuadtreeNode(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 halfWD, std::vector<BoundingVolume*> models, unsigned int levels, unsigned int currentLevel);
	~QuadtreeNode();
};