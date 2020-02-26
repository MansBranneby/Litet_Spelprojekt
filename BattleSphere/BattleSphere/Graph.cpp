#include "Graph.h"

Graph* Graph::m_instance = nullptr;

std::vector<XMVECTOR> Graph::pointFiller(int index, std::vector<XMVECTOR> path)
{
	std::vector<XMVECTOR> points;
	float rest = 0;
	for (int i = 0; i < path.size(); i++)
	{
		/*float length = XMVectorGetX(XMVector3Length(path[i] - path[i + (int)1]));
		float currDist = POINT_DISTANCE - rest;
		rest = std::fmodf(length - currDist, POINT_DISTANCE);
		//int nrOfPoints = floorf(length / POINT_DISTANCE);

		while (currDist < length)
		{
			points.push_back(XMVectorLerp(path[i], path[i + (int)1],
				currDist / length
			));
			currDist += POINT_DISTANCE;
		}*/
		path[i].m128_f32[1] = 1.2f + 0.05f * index;
		points.push_back(path[i]);
		
	}
	m_path[index] = points;



	return points;
}

void Graph::createVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(XMVECTOR) * 25;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	/*D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &m_path[0];*/

	// create a Vertex Buffer
	HRESULT result = DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, nullptr, &m_vsBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Path VB", L"Error", MB_OK | MB_ICONERROR);
}

bool Graph::getActive(int index)
{
	return m_active[index];
}

void Graph::setQuadtree(QuadtreeNode* qtn)
{
	m_quadtree = qtn;
}

Graph::Graph()
{
	for (int i = 0; i < 39; i++)
	{
		m_nodes.push_back(graphNode());

	}
	m_nodes[0].pos = XMVectorSet(150.0f, 0.0f, 120.0f, 0.0f);
	m_nodes[1].pos = XMVectorSet(106.0f, 0.0f, -18.0f, 0.0f);
	m_nodes[2].pos = XMVectorSet(-100.0f, 0.0f, -50.0f, 0.0f);
	m_nodes[3].pos = XMVectorSet(-120.0f, 0.0f, -12.0f, 0.0f);
	m_nodes[4].pos = XMVectorSet(156.0f, 0.0f, 13.0f, 0.0f);
	m_nodes[5].pos = XMVectorSet(156.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[6].pos = XMVectorSet(106.0f, 0.0f, -58.0f, 0.0f);
	m_nodes[7].pos = XMVectorSet(122.0f, 0.0f, -78.0f, 0.0f);
	m_nodes[8].pos = XMVectorSet(102.0f, 0.0f, -110.0f, 0.0f);
	m_nodes[9].pos = XMVectorSet(80.0f, 0.0f, -60.0f, 0.0f);
	m_nodes[10].pos = XMVectorSet(66.0f, 0.0f, -66.0f, 0.0f);
	m_nodes[11].pos = XMVectorSet(66.0f, 0.0f, -38.0f, 0.0f);
	m_nodes[12].pos = XMVectorSet(27.0f, 0.0f, -38.0f, 0.0f);
	m_nodes[13].pos = XMVectorSet(16.0f, 0.0f, -7.0f, 0.0f);
	m_nodes[14].pos = XMVectorSet(27.0f, 0.0f, -66.0f, 0.0f);
	m_nodes[15].pos = XMVectorSet(44.0f, 0.0f, -70.0f, 0.0f);
	m_nodes[16].pos = XMVectorSet(14.0f, 0.0f, -75.0f, 0.0f);
	m_nodes[17].pos = XMVectorSet(0.0f, 0.0f, -110.0f, 0.0f);
	m_nodes[18].pos = XMVectorSet(-14.0f, 0.0f, -75.0f, 0.0f);
	m_nodes[19].pos = XMVectorSet(-65.0f, 0.0f, -92.0f, 0.0f);
	m_nodes[20].pos = XMVectorSet(120.0f, 0.0f, 120.0f, 0.0f);
	m_nodes[21].pos = XMVectorSet(-65.0f, 0.0f, -45.0f, 0.0f);
	m_nodes[22].pos = XMVectorSet(-14.0f, 0.0f, -45.0f, 0.0f);
	m_nodes[23].pos = XMVectorSet(-14.0f, 0.0f, 0.0f, 0.0f);
	m_nodes[24].pos = XMVectorSet(-49.0f, 0.0f, 0.0f, 0.0f);
	m_nodes[25].pos = XMVectorSet(-65.0f, 0.0f, -16.0f, 0.0f);
	m_nodes[26].pos = XMVectorSet(-65.0f, 0.0f, 16.0f, 0.0f);
	m_nodes[27].pos = XMVectorSet(-65.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[28].pos = XMVectorSet(-97.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[29].pos = XMVectorSet(-120.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[30].pos = XMVectorSet(120.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[31].pos = XMVectorSet(-120.0f, 0.0f, 120.0f, 0.0f);
	m_nodes[32].pos = XMVectorSet(-97.0f, 0.0f, 120.0f, 0.0f);
	m_nodes[33].pos = XMVectorSet(-60.0f, 0.0f, 85.0f, 0.0f);
	m_nodes[34].pos = XMVectorSet(-30.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[35].pos = XMVectorSet(-30.0f, 0.0f, 120.0f, 0.0f);
	m_nodes[36].pos = XMVectorSet(-14.0f, 0.0f, 120.0f, 0.0f);
	m_nodes[37].pos = XMVectorSet(-14.0f, 0.0f, 50.0f, 0.0f);
	m_nodes[38].pos = XMVectorSet(16.0f, 0.0f, 50.0f, 0.0f);

	m_nodes[0].neighbours = std::vector<int>{ 21 };
	m_nodes[1].neighbours = std::vector<int>{ 5, 7 };
	m_nodes[2].neighbours = std::vector<int>{ 20, 22 };
	m_nodes[3].neighbours = std::vector<int>{ 30 };
	m_nodes[4].neighbours = std::vector<int>{ 2, 6 };
	m_nodes[5].neighbours = std::vector<int>{ 31, 5 };
	m_nodes[6].neighbours = std::vector<int>{ 2, 8, 10 };
	m_nodes[7].neighbours = std::vector<int>{ 7, 9 };
	m_nodes[8].neighbours = std::vector<int>{ 8, 16, 18 };
	m_nodes[9].neighbours = std::vector<int>{ 7, 11, 12 };
	m_nodes[10].neighbours = std::vector<int>{ 10, 12, 15, 16 };
	m_nodes[11].neighbours = std::vector<int>{ 10, 11, 13 };
	m_nodes[12].neighbours = std::vector<int>{ 12, 14, 15 };
	m_nodes[13].neighbours = std::vector<int>{ 13, 17, 39 };
	m_nodes[14].neighbours = std::vector<int>{ 11, 13, 17 };
	m_nodes[15].neighbours = std::vector<int>{ 9, 11, 17, 18 };
	m_nodes[16].neighbours = std::vector<int>{ 14, 15, 16, 18, 19 };
	m_nodes[17].neighbours = std::vector<int>{ 9, 16, 17, 19, 20 };
	m_nodes[18].neighbours = std::vector<int>{ 17, 18, 22, 23 };
	m_nodes[19].neighbours = std::vector<int>{ 3, 18 };
	m_nodes[20].neighbours = std::vector<int>{ 1, 31 };
	m_nodes[21].neighbours = std::vector<int>{ 3, 19, 23, 26 };
	m_nodes[22].neighbours = std::vector<int>{ 19, 22, 24 };
	m_nodes[23].neighbours = std::vector<int>{ 23, 25, 38 };
	m_nodes[24].neighbours = std::vector<int>{ 24, 26, 27 };
	m_nodes[25].neighbours = std::vector<int>{ 22, 25, 27 };
	m_nodes[26].neighbours = std::vector<int>{ 25, 26, 28 };
	m_nodes[27].neighbours = std::vector<int>{ 27, 29, 35 };
	m_nodes[28].neighbours = std::vector<int>{ 28, 30, 34 };
	m_nodes[29].neighbours = std::vector<int>{ 4, 29, 32 };
	m_nodes[30].neighbours = std::vector<int>{ 6, 21, 39 };
	m_nodes[31].neighbours = std::vector<int>{ 30, 33 };
	m_nodes[32].neighbours = std::vector<int>{ 32, 34, 36 };
	m_nodes[33].neighbours = std::vector<int>{ 29, 33, 35, 36 };
	m_nodes[34].neighbours = std::vector<int>{ 28, 34, 38 };
	m_nodes[35].neighbours = std::vector<int>{ 33, 34, 37 };
	m_nodes[36].neighbours = std::vector<int>{ 36, 38 };
	m_nodes[37].neighbours = std::vector<int>{ 24, 35, 37, 39 };
	m_nodes[38].neighbours = std::vector<int>{ 14, 31, 38 };
	//index = Lights::getInstance()->addPointLight(0, 0, 0, 20, 1, 1, 1, 20);

	for (int i = 0; i < m_nodes.size(); i++)
	{
		for (int j = 0; j < m_nodes[i].neighbours.size(); j++)
		{
			m_nodes[i].neighbourDistance.push_back(
				XMVectorGetX(XMVector3Length(
					m_nodes[i].pos - m_nodes[--m_nodes[i].neighbours[j]].pos
				))
			);
		}
		for (int j = 0; j < 4; j++)
		{
			m_nodes[i].heuristic[j] = XMVectorGetX(XMVector3Length(
				m_nodes[i].pos - m_nodes[j].pos
			));
		}
	}
	for (int i = 0; i < 4; i++)
	{
		m_path[i].push_back(XMVectorSet(0, 0, 0, 0));
		m_active[i] = false;
		m_pulseLength[i] = 0.0f;

	}
	m_pulseCBuffer = new ConstantBuffer(&m_pulsePos[0], sizeof(XMVECTOR));

	m_vs = VertexShader(L"VertexShaderPath.hlsl", 2);
	m_gs = GeometryShader(L"GeometryShaderPath.hlsl");
	m_ps = PixelShader(L"PixelShaderPath.hlsl");
}

Graph* Graph::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Graph();
	}

	return m_instance;
}

void Graph::updatePulse(int index, float dt)
{
	bool newPulse = true;
	m_pulseLength[index] += PULSE_SPEED * dt;
	float distance = m_pulseLength[index];
	for (int i = m_path[index].size() - 2; i >= 0; i--)
	{
		float length = XMVectorGetX(XMVector3Length(m_path[index][i] - m_path[index][i + (int)1]));
		if (distance < length)
		{
			m_pulsePos[index] = XMVectorLerp(m_path[index][i + 1], m_path[index][i], distance / length);
			newPulse = false;
			break;
		}
		distance -= length;

	}
	if (newPulse)
	{
		m_pulseLength[index] = 0;
		m_active[index] = false;
		//calculateShortestPath(XMVectorSet(120, 0, 0, 0), 0);
	}
	
	//		Lights::getInstance()->setPosition(index, m_pulsePos.m128_f32[0], 5, m_pulsePos.m128_f32[2]);
}

std::vector<XMVECTOR> Graph::calculateShortestPath(int index, XMVECTOR startPos, int goal)
{
	if (!m_active[index])
	{
		m_active[index] = true;
		struct node
		{
			int index;
			int source;
			float f, g;
		};

		std::vector<node> open;
		std::vector<node> closed;
		float shortestDistance = 10000.0f;
		int startNode = -1;
		float heuristic = XMVectorGetX(XMVector3Length(m_nodes[goal].pos - startPos));
		XMFLOAT2 playerPos;
		playerPos.x = startPos.m128_f32[0];
		playerPos.y = startPos.m128_f32[2];
		for (int i = 0; i < m_nodes.size(); i++)
		{

			float currDist = XMVectorGetX(XMVector3Length(m_nodes[i].pos - startPos));
			if (currDist < shortestDistance)
			{
				XMFLOAT2 nodePos;
				nodePos.x = m_nodes[i].pos.m128_f32[0];
				nodePos.y = m_nodes[i].pos.m128_f32[2];
				if (!m_quadtree->testCollision(playerPos, nodePos))
				{
					shortestDistance = currDist;
					startNode = i;
				}

			}
		}
		if (startNode == -1)
		{
			m_active[index] = false;
			return std::vector<XMVECTOR>();
		}
		

		//nodes.push_back({ startNode, -1, 0});
		open.push_back({ startNode, -1, 0, 0 });



		while (open.size() > 0)
		{
			float lowestF = 100000;
			int q = 0;
			//Find the element with the lowest f value
			for (int i = 0; i < open.size(); i++)
			{
				if (open[i].f < lowestF)
				{
					//you are the one
					lowestF = open[i].f;
					q = i;
				}
			}
			//Are we the goal????
			if (m_nodes[open[q].index].heuristic[goal] < 1.0f)
			{
				std::vector<XMVECTOR> path;
				node currentNode = open[q];
				while (currentNode.source != -1)
				{
					path.push_back(m_nodes[currentNode.index].pos);
					currentNode = closed[currentNode.source];
				}
				path.push_back(m_nodes[startNode].pos);
				for (int i = 0; i < path.size(); i++)
				{
					XMFLOAT2 nodePos;
					nodePos.x = path[i].m128_f32[0];
					nodePos.y = path[i].m128_f32[2];
					if (!m_quadtree->testCollision(playerPos, nodePos))
					{
						path.erase(path.begin() + (i + 1), path.end());
						break;
					}
				}
				path.push_back(startPos);
				return pointFiller(index, path);
			}


			//Add the children
			for (int i = 0; i < m_nodes[open[q].index].neighbours.size(); i++)
			{
				node nod;
				nod.source = closed.size();
				nod.index = m_nodes[open[q].index].neighbours[i];
				nod.g = open[q].g + m_nodes[open[q].index].neighbourDistance[i];
				nod.f = nod.g + m_nodes[nod.index].heuristic[goal];

				bool add = true;
				for (int j = 0; j < closed.size(); j++)
				{
					if (nod.index == closed[j].index)
					{
						add = false;
					}
				}
				for (int j = 0; j < open.size(); j++)
				{
					if (nod.index == open[j].index)
					{
						if (nod.g > open[j].g)
							add = false;
					}
				}
				if (add)
				{
					open.push_back(nod);
				}

			}
			closed.push_back(open[q]);
			open.erase(open.begin() + q);

		}
	}
	return std::vector<XMVECTOR>();
}

void Graph::draw(int index)
{
	if (m_active[index])
	{
		D3D11_MAPPED_SUBRESOURCE mappedMemory;
		if (m_path[index].size() > 0)
		{
			DX::getInstance()->getDeviceContext()->Map(m_vsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
			memcpy(mappedMemory.pData, &m_path[index][0], sizeof(XMVECTOR) * m_path[index].size());
			DX::getInstance()->getDeviceContext()->Unmap(m_vsBuffer, 0);


		}
		DX::getInstance()->getDeviceContext()->Map(*m_pulseCBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_pulsePos[index], sizeof(XMVECTOR));
		DX::getInstance()->getDeviceContext()->Unmap(*m_pulseCBuffer->getConstantBuffer(), 0);

		DX::getInstance()->getDeviceContext()->VSSetShader(&m_vs.getVertexShader(), nullptr, 0);
		DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
		DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
		DX::getInstance()->getDeviceContext()->GSSetShader(&m_gs.getGeometryShader(), nullptr, 0);
		DX::getInstance()->getDeviceContext()->PSSetShader(&m_ps.getPixelShader(), nullptr, 0);
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_pulseCBuffer->getConstantBuffer());
		UINT32 vertexSize = sizeof(XMVECTOR);
		UINT32 offset = 0;

		DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vsBuffer, &vertexSize, &offset);
		DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		DX::getInstance()->getDeviceContext()->IASetInputLayout(&m_vs.getvertexLayout());
		DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);
		DX::getInstance()->getDeviceContext()->Draw(m_path[index].size(), 0);
	}
	

}

void Graph::release()
{
	m_vs.release();
	m_gs.release();
	m_ps.release();
	m_vsBuffer->Release();
	delete m_pulseCBuffer;
}
