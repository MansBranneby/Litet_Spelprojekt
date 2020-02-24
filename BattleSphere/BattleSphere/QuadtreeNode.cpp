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
	// Create this node's bounding volume
	m_boundingData.pos = { 0.0f, 0.0f, 0.0f };
	m_boundingData.halfWD = { 0.0f, 0.0f };
	m_boundingData.xAxis = { 1.0f, 0.0f, 0.0f };
	m_boundingData.zAxis = { 0.0f, 0.0f, 1.0f };
}

QuadtreeNode::QuadtreeNode(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 halfWD, PreLoader* preLoader, unsigned int levels, unsigned int currentLevel)
{
	// Create this node's bounding volume
	m_boundingData.pos = DirectX::XMLoadFloat3(&pos);
	m_boundingData.halfWD = halfWD;
	m_boundingData.xAxis = { 1.0f, 0.0f, 0.0f };
	m_boundingData.zAxis = { 0.0f, 0.0f, 1.0f };
	
	// Calculate 4 new node positions
	std::vector<DirectX::XMFLOAT3> nodePositions = calculateNewNodePositions(pos, halfWD); 

	// Get bounding data to create bounding volume
	// Test collision of node OBB and other OBB
	for (int i = 0; i < preLoader->getNrOfVariants(ObjectType::e_static); ++i)
	{
		boundingData boundingData = preLoader->getBoundingData(ObjectType::e_static, 0, i);

		// Test collision
		if (testOBBOBB(m_boundingData, boundingData).m_colliding)
		{
			// Append collisionMesh to to m_cMeshes
			std::vector<DirectX::XMFLOAT3> tempCMesh = preLoader->getCollisionMesh(ObjectType::e_static, 0, i);
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
}

CollisionInfo QuadtreeNode::testCollision(boundingData boundingVolume)
{
	CollisionInfo collisionInfo;

	// Test if other intersects with this node
	if (testOBBOBB(m_boundingData, boundingVolume).m_colliding)
	{
		// If this node has no children then start triangle intersection test with other
		if (m_children.size() == 0)
		{
			unsigned int nrOfCollisions = 0;
			DirectX::XMVECTOR normal{ 0.0f, 0.0f, 0.0f, 0.0f };

			for (unsigned int i = 0; i < m_cMeshes.size(); i += 3)
			{
				unsigned int ind1 = i + 1;
				unsigned int ind2 = i + 2;

				collisionInfo = testSphereTriangle(boundingVolume.pos, boundingVolume.halfWD.x,
					XMVECTOR{ m_cMeshes[i].x, m_cMeshes[i].y, m_cMeshes[i].z },
					XMVECTOR{ m_cMeshes[ind1].x, m_cMeshes[ind1].y, m_cMeshes[ind1].z },
					XMVECTOR{ m_cMeshes[ind2].x, m_cMeshes[ind2].y, m_cMeshes[ind2].z });
				// Collision detected!
				if (collisionInfo.m_colliding)
				{
					// Average normals in case bounding volume intersects with corner

					normal = collisionInfo.m_normal;
					nrOfCollisions++;

					// Early return in case collision with two triangles detected
					if (nrOfCollisions >= 2)
					{
						collisionInfo.m_normal = normal;
						return collisionInfo;
					}
				}
			}

			if (nrOfCollisions >= 1)
			{
				collisionInfo.m_colliding = true;
				collisionInfo.m_normal = normal;
			}

		}
		else
		{
			// Recursive function call testCollision
			for (unsigned int i = 0; i < m_children.size(); ++i)
			{
				collisionInfo = m_children[i]->testCollision(boundingVolume);
				if (collisionInfo.m_colliding) return collisionInfo;
			}
		}
	}

	return collisionInfo;
}

CollisionInfo QuadtreeNode::testCollision(boundingData boundingVolume, DirectX::XMVECTOR previousPos)
{
	CollisionInfo collisionInfo;

	// Test if other intersects with this node
	if (testOBBSphere(boundingVolume.pos, boundingVolume.halfWD.x, m_boundingData).m_colliding)
	{	
		// If this node has no children then start triangle intersection test with other
		if (m_children.size() == 0)
		{
			unsigned int nrOfCollisions = 0;
			float shortest = INFINITY;
			float length = 0.0f;

			std::vector<DirectX::XMVECTOR> normals;
			std::vector<DirectX::XMVECTOR> contactPoints;

			for (int i = 0; i < m_cMeshes.size(); i += 3)
			{
				int ind1 = i + 1;
				int ind2 = i + 2;

				collisionInfo = testSphereTriangle(boundingVolume.pos, boundingVolume.halfWD.x,
					XMVECTOR{ m_cMeshes[i].x, m_cMeshes[i].y, m_cMeshes[i].z },
					XMVECTOR{ m_cMeshes[ind1].x, m_cMeshes[ind1].y, m_cMeshes[ind1].z },
					XMVECTOR{ m_cMeshes[ind2].x, m_cMeshes[ind2].y, m_cMeshes[ind2].z });


				// Collision detected!
				if (collisionInfo.m_colliding)
				{
					// Average normals in case bounding volume intersects with corner					
					normals.push_back(collisionInfo.m_normal);
					contactPoints.push_back(collisionInfo.m_contactPoint);
					nrOfCollisions++;
				}
			}

			// Find normal closest to current position
			for (int j = 0; j < normals.size(); ++j)
			{
				length = DirectX::XMVectorGetX(DirectX::XMVector3Length(contactPoints[j] - boundingVolume.pos));

				if (length < shortest)
				{
					shortest = length;
					collisionInfo.m_normal = normals[j];
					collisionInfo.m_colliding = true;
				}
			}	
		}
		else
		{
			// Recursive function call testCollision
			for (unsigned int i = 0; i < m_children.size(); ++i)
			{
				collisionInfo = m_children[i]->testCollision(boundingVolume, previousPos);
				if (collisionInfo.m_colliding) return collisionInfo;
			}
		}
	}

	return collisionInfo;
}
