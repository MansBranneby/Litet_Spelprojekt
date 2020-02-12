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

QuadtreeNode::QuadtreeNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, PreLoader preLoader, unsigned int levels, unsigned int currentLevel)
{
	m_boundingVolume = new OBB(DirectX::XMLoadFloat3(&pos), halfWD, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	std::vector<DirectX::XMFLOAT3> nodePositions = calculateNewNodePositions(pos, halfWD);

	// TODO replace with models instead of bounding volumes
	boundingData bd = preLoader.getBoundingData(objectType::e_scene, 0, 0);
	OBB tempOBB(bd.pos, bd.halfWD, bd.xAxis, bd.zAxis);
	for (unsigned int i = 0; i < 1; ++i)
	{
		if (m_boundingVolume->intersects(&tempOBB).m_colliding)
		{
			// Append to m_cMeshes
			std::vector<DirectX::XMFLOAT3> tempCMesh = preLoader.getCollisionMesh(objectType::e_scene, 0, 0);
			m_cMeshes.insert(std::end(m_cMeshes), std::begin(tempCMesh), std::end(tempCMesh));
		}
	}

	// Recursive creation of nodes
	if (currentLevel < levels)
	{
		halfWD.x *= 0.5f;
		halfWD.y *= 0.5f;
		for (size_t i = 0; i < 4; i++)
			m_children.push_back(new QuadtreeNode(nodePositions[i], halfWD, preLoader, levels, currentLevel + 1));
	}
}

QuadtreeNode::~QuadtreeNode()
{
	for (size_t i = 0; i < m_children.size(); i++)
		delete m_children[i];

	delete m_boundingVolume;
}
