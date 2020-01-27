#pragma once
#include <vector>
#include <DirectXMath.h>

class QuadtreeNode
{
private:
	std::vector <QuadtreeNode*> m_children;

	DirectX::XMFLOAT4 calculateBox();

	//bool intersectWith();

public:
	QuadtreeNode();
	//	Dimensions of box
	QuadtreeNode(DirectX::XMFLOAT4 minMaxXZ, std::vector <DirectX::XMFLOAT3> meshPos, unsigned int levels, unsigned int currentLevel);
	~QuadtreeNode();
};