#include "QuadtreeNode.h"

DirectX::XMFLOAT4 QuadtreeNode::calculateBox()
{
	return DirectX::XMFLOAT4(1.0, 1.0, 1.0, 1.0);
}

QuadtreeNode::QuadtreeNode()
{

}

QuadtreeNode::QuadtreeNode(DirectX::XMFLOAT4 minMaxXZ, std::vector <DirectX::XMFLOAT3> meshPos, unsigned int levels, unsigned int currentLevel)
{
	
}

QuadtreeNode::~QuadtreeNode()
{

}
