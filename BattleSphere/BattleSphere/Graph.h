#pragma once

#include "StructsAndEnums.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "QuadtreeNode.h"

#define POINT_DISTANCE 1
#define PULSE_SPEED 120
#define MAX_USER 4

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
	int m_goal[4];
	std::vector<XMVECTOR> m_path[MAX_USER];
	XMVECTOR m_pulsePos[MAX_USER];
	float m_pulseLength[MAX_USER];
	float m_pulseCounter[MAX_USER];
	bool m_active[MAX_USER];
	XMVECTOR m_colour[MAX_USER];
	VertexShader m_vs;
	GeometryShader m_gs;
	PixelShader m_ps;
	ConstantBuffer* m_pulseCBuffer;
	ConstantBuffer* m_colourCBuffer;
	ID3D11Buffer* m_vsBuffer = nullptr;
	QuadtreeNode* m_quadtree;

	

public:
	Graph();
	
	static Graph* getInstance();
	void updatePulse(int index, float dt);
	void createVertexBuffer();
	bool getActive(int index);
	void setQuadtree(QuadtreeNode* qtn);
	void setColour(int index, XMVECTOR colour);
	std::vector<XMVECTOR> calculateAIPath(XMVECTOR startPos, XMVECTOR goal);
	std::vector<XMVECTOR> calculateShortestPath(int index, XMVECTOR startPos, int goal);
	void setShortestPath(int index, std::vector<XMVECTOR>* path);
	void draw(int index);
	XMVECTOR getNodePos(int index);

	void reset(int index);
	void release();
};

