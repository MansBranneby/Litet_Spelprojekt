#include "QuadtreeNode.h"

std::vector<DirectX::XMFLOAT2> QuadtreeNode::calculateNewNodePositions(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 halfWD)
{
	// Calculate new positions
	halfWD.x *= 0.5f;
	halfWD.y *= 0.5f;

	std::vector<DirectX::XMFLOAT2> nodePositions =
	{
		DirectX::XMFLOAT2(pos.x + halfWD.x, pos.y),
		DirectX::XMFLOAT2(pos.x - halfWD.x, pos.y),
		DirectX::XMFLOAT2(pos.x, pos.y + halfWD.y),
		DirectX::XMFLOAT2(pos.x, pos.y - halfWD.y)
	};

	return nodePositions;
}

QuadtreeNode::QuadtreeNode()
{
	m_boundingVolume = new OBB({ 0.0, 0.0 }, { 0.0, 0.0 }, DirectX::XMMatrixIdentity());
}

QuadtreeNode::QuadtreeNode(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 halfWD, std::vector<BoundingVolume*> models, unsigned int levels, unsigned int currentLevel)
{
	m_boundingVolume = new OBB(pos, halfWD, DirectX::XMMatrixIdentity());
	std::vector<DirectX::XMFLOAT2> nodePositions = calculateNewNodePositions(pos, halfWD);
	
	// TODO replace with models instead of bounding volumes
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (m_boundingVolume->intersects(models[i]).m_colliding)
			m_models.push_back(models[i]);
	}

	// Recursive creation of nodes
	if (currentLevel < levels)
	{
		for (size_t i = 0; i < 4; i++)
			m_children.push_back(new QuadtreeNode(nodePositions[i], halfWD , m_models, levels, currentLevel + 1));
	}
}

QuadtreeNode::~QuadtreeNode()
{
	for (size_t i = 0; i < m_children.size(); i++)
		delete m_children[i];

	for (size_t i = 0; i < m_models.size(); i++)
		delete m_models[i];
}
