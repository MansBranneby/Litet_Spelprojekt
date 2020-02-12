#include "QuadtreeNode.h"

std::vector<DirectX::XMFLOAT3> QuadtreeNode::calculateNewNodePositions(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD)
{
	// Calculate new positions
	std::vector<DirectX::XMFLOAT3> nodePositions =
	{		
		DirectX::XMFLOAT3(pos.x + halfWD.x * 0.5f, 0.0f, pos.z + halfWD.y * 0.5f), // Right far
		DirectX::XMFLOAT3(pos.x + halfWD.x * 0.5f, 0.0f, pos.z - halfWD.y * 0.5f), // Right near
		DirectX::XMFLOAT3(pos.x - halfWD.x * 0.5f, 0.0f, pos.z + halfWD.y * 0.5f), // Left far
		DirectX::XMFLOAT3(pos.x - halfWD.x * 0.5f, 0.0f, pos.z - halfWD.y * 0.5f)  // Left near
	};

	return nodePositions;
}

QuadtreeNode::QuadtreeNode()
{
	m_boundingVolume = new OBB({ 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f});
}

QuadtreeNode::QuadtreeNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, std::vector<BoundingVolume*> models, unsigned int levels, unsigned int currentLevel)
{
	m_boundingVolume = new OBB(DirectX::XMLoadFloat3(&pos), halfWD, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	std::vector<DirectX::XMFLOAT3> nodePositions = calculateNewNodePositions(pos, halfWD);
	
	// TODO replace with models instead of bounding volumes
	for (unsigned int i = 0; i < models.size(); ++i)
	{
		if (m_boundingVolume->intersects(models[i]).m_colliding)
			m_models.push_back(models[i]);
	}

	// Recursive creation of nodes
	if (currentLevel < levels)
	{
		halfWD.x *= 0.5f;
		halfWD.y *= 0.5f;
		for (size_t i = 0; i < 4; i++)
			m_children.push_back(new QuadtreeNode(nodePositions[i], halfWD, m_models, levels, currentLevel + 1));
	}
}

QuadtreeNode::~QuadtreeNode()
{
	for (size_t i = 0; i < m_children.size(); i++)
		delete m_children[i];

	delete m_boundingVolume;
}
