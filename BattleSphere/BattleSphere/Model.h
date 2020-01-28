#pragma once
#include "SubModel.h"
#include <string>
#include <fstream>
#include <sstream>

struct vertex {
	float posX, posY, posZ;
	float u, v;
	float normX, normY, normZ;
};

class Model
{
private:
	int m_nrOfVertices;
	vertex* m_vertices;
	int m_nrOfSubModels;
	SubModel* m_subModels;

	ID3D11Buffer* m_vertexBuffer;

	void createVertexBuffer();

public:
	void draw();
	Model();
	~Model();

	void loadModel(std::ifstream& in);
};

