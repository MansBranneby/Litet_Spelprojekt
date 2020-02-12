#include "QuadtreeNode.h"

std::vector<DirectX::XMFLOAT3> QuadtreeNode::calculateNewNodePositions(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD)
{
	// Calculate new positions
	std::vector<DirectX::XMFLOAT3> nodePositions =
	{		
		DirectX::XMFLOAT3(pos.x - halfWD.x * 0.5f, 0.0f, pos.z + halfWD.y * 0.5f), // Left far
		DirectX::XMFLOAT3(pos.x + halfWD.x * 0.5f, 0.0f, pos.z + halfWD.y * 0.5f), // Right far
		DirectX::XMFLOAT3(pos.x - halfWD.x * 0.5f, 0.0f, pos.z - halfWD.y * 0.5f), // Left near
		DirectX::XMFLOAT3(pos.x + halfWD.x * 0.5f, 0.0f, pos.z - halfWD.y * 0.5f)  // Right near
	};

	return nodePositions;
}

QuadtreeNode::QuadtreeNode()
{
	m_boundingVolume = new OBB({ 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f});
}

QuadtreeNode::QuadtreeNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, PreLoader* preLoader, unsigned int levels, unsigned int currentLevel)
{
	// Create this node's bounding volume
	m_boundingVolume = new OBB(DirectX::XMLoadFloat3(&pos), halfWD, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }); 
	
	// Calculate 4 new node positions
	std::vector<DirectX::XMFLOAT3> nodePositions = calculateNewNodePositions(pos, halfWD); 

	// Get bounding data to create bounding volume
	boundingData bd = preLoader->getBoundingData(objectType::e_scene, 0, 0);
	OBB tempOBB(bd.pos, bd.halfWD, bd.xAxis, bd.zAxis);
	// Test collision of node OBB and other OBB
	for (unsigned int i = 0; i < 1; ++i)
	{
		// Test collision
		if (m_boundingVolume->intersects(&tempOBB).m_colliding)
		{
			// Append collisionMesh to to m_cMeshes
			std::vector<DirectX::XMFLOAT3> tempCMesh = preLoader->getCollisionMesh(objectType::e_scene, 0, 0);
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

CollisionInfo QuadtreeNode::testCollision(BoundingVolume* other)
{
	CollisionInfo collisionInfo;

	// Test if other intersects with this node
	if (m_boundingVolume->intersects(other).m_colliding)
	{	
		// If this node has no children then start triangle intersection test with other
		if (m_children.size() == 0)
		{
			for (unsigned int i = 0; i < m_cMeshes.size(); i += 3)
			{
				collisionInfo = other->intersectsWithTriangle(
					XMVECTOR{ m_cMeshes[i].x, m_cMeshes[i].y, m_cMeshes[i].z },
					XMVECTOR{ m_cMeshes[i + 1.0].x, m_cMeshes[i + 1.0].y, m_cMeshes[i + 1.0].z },
					XMVECTOR{ m_cMeshes[i + 2.0].x, m_cMeshes[i + 2.0].y, m_cMeshes[i + 2.0].z });
				// Collision detected, return information
				if (collisionInfo.m_colliding)
					return collisionInfo;
			}
		}
		else
		{
			// Recursive function call testCollision
			for (unsigned int i = 0; i < m_children.size(); ++i)
				m_children[i]->testCollision(other);
		}
	}

	return collisionInfo;
}
