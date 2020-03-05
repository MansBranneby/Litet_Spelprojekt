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
		for (int j = 0; j < preLoader->getNrOfModels(ObjectType::e_static, i); j++)
		{
			boundingData boundingData = preLoader->getBoundingData(ObjectType::e_static, j, i);

			// Test collision
			if (testOBBOBB(m_boundingData, boundingData).m_colliding)
			{
				// Append collisionMesh to to m_cMeshes
				std::vector<DirectX::XMFLOAT3> tempCMesh = preLoader->getCollisionMesh(ObjectType::e_static, j, i);
				m_cMeshes.insert(std::end(m_cMeshes), std::begin(tempCMesh), std::end(tempCMesh));
			}
		}
	}
	// Check static billboards
	for (int i = 0; i < preLoader->getNrOfVariants(ObjectType::e_static_billboard); ++i)
	{
		for (int j = 0; j < preLoader->getNrOfModels(ObjectType::e_static_billboard, i); j++)
		{
			boundingData boundingData = preLoader->getBoundingData(ObjectType::e_static_billboard, j, i);

			// Test collision
			if (testOBBOBB(m_boundingData, boundingData).m_colliding)
			{
				// Append collisionMesh to to m_cMeshes
				std::vector<DirectX::XMFLOAT3> tempCMesh = preLoader->getCollisionMesh(ObjectType::e_static_billboard, j, i);
				m_cMeshes.insert(std::end(m_cMeshes), std::begin(tempCMesh), std::end(tempCMesh));
			}
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

bool QuadtreeNode::testCollision(XMFLOAT2 start, XMFLOAT2 end)
{
	
	XMFLOAT2 point[4];
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, m_boundingData.pos - m_boundingData.halfWD.x * m_boundingData.xAxis + m_boundingData.halfWD.y * m_boundingData.zAxis);//Top Left;
	point[0].x = pos.x;
	point[0].y = pos.z;
	XMStoreFloat3(&pos, m_boundingData.pos + m_boundingData.halfWD.x * m_boundingData.xAxis + m_boundingData.halfWD.y * m_boundingData.zAxis);//Top Right;
	point[1].x = pos.x;
	point[1].y = pos.z;
	XMStoreFloat3(&pos, m_boundingData.pos - m_boundingData.halfWD.x * m_boundingData.xAxis - m_boundingData.halfWD.y * m_boundingData.zAxis);//Bottom Left;
	point[2].x = pos.x;
	point[2].y = pos.z;
	XMStoreFloat3(&pos, m_boundingData.pos + m_boundingData.halfWD.x * m_boundingData.xAxis - m_boundingData.halfWD.y * m_boundingData.zAxis);//Bottom Right;
	point[3].x = pos.x;
	point[3].y = pos.z;
	bool collision = false;
	if (start.x < point[1].x && start.x > point[0].x && start.y > point[2].y && start.y < point[0].y)
		collision = true;

	if(!collision)
		collision = testLineLine(XMLoadFloat2(&start), XMLoadFloat2(&end), XMLoadFloat2(&point[1]), XMLoadFloat2(&point[3]));
	if (!collision)
		collision = testLineLine(XMLoadFloat2(&start), XMLoadFloat2(&end), XMLoadFloat2(&point[0]), XMLoadFloat2(&point[2]));
	if (!collision)
		collision = testLineLine(XMLoadFloat2(&start), XMLoadFloat2(&end), XMLoadFloat2(&point[0]), XMLoadFloat2(&point[1]));
	if (!collision)
		collision = testLineLine(XMLoadFloat2(&start), XMLoadFloat2(&end), XMLoadFloat2(&point[2]), XMLoadFloat2(&point[3]));

	
	if (collision)
	{
		collision = false;
		if (m_children.size() == 0)
		{
			//Epic triangle collision detection :OO
			//collision
			for (int i = 0; i < m_cMeshes.size(); i+=3)
			{
				int ind1 = i + 1;
				int ind2 = i + 2;

				XMVECTOR triangles[3];
				triangles[0] = XMLoadFloat3(&m_cMeshes[i]);
				triangles[1] = XMLoadFloat3(&m_cMeshes[ind1]);
				triangles[2] = XMLoadFloat3(&m_cMeshes[ind2]);

				XMVECTOR startReal = { start.x, 0.5f, start.y };
				XMVECTOR endReal = { end.x, 0.5f, end.y };

				collision = testLineTriangle(startReal, endReal, triangles);
				if (collision)
					return true;
			}
		}
		else
		{
			for (int i = 0; i < m_children.size(); i++)
			{
				collision = m_children[i]->testCollision(start, end);
				if (collision)
					return true;
			}
		}
	}
	return false;
}
