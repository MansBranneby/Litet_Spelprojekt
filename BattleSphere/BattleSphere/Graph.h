#pragma once

#include "StructsAndEnums.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "VertexShader.h"

#define POINT_DISTANCE 9
#define PULSE_SPEED 90

struct graphNode
{
	XMVECTOR pos;
	std::vector<int> neighbours;
	std::vector<float> neighbourDistance;
	float heuristic[4];
};

class Graph
{
private:
	static Graph* m_instance;

	std::vector<graphNode> m_nodes;
	std::vector<XMVECTOR> m_path;
	VertexShader m_vs;
	GeometryShader m_gs;
	PixelShader m_ps;
	ConstantBuffer* m_pulseCBuffer;
	ID3D11Buffer* m_vsBuffer = nullptr;
	XMVECTOR m_pulsePos;
	float m_pulseLength;
	bool m_active;

	//Remove
	int index;

	std::vector<XMVECTOR> pointFiller(std::vector<XMVECTOR> path);
public:
	Graph();
	
	static Graph* getInstance();
	void updatePulse(float dt);
	void createVertexBuffer();
	bool getActive();
	std::vector<XMVECTOR> calculateShortestPath(XMVECTOR startPos, int goal);
	void draw();

	void release();
};

