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
	XMVECTOR m_pos;
	XMVECTOR m_posRelative;
	int m_nrOfVertices;
	vertex* m_vertices;
	int m_nrOfSubModels;
	SubModel* m_subModels;

	XMMATRIX m_modelMatrix;

	ID3D11Buffer* m_modelMatrixCBuffer;
	XMMATRIX* m_modelMatrixData;
	XMMATRIX m_rotationMat;
	XMMATRIX m_rotationAfterMat;
	XMMATRIX m_scalingMat;
	ID3D11Buffer* m_vertexBuffer;

	void createVertexBuffer(); // For vertex buffer
	void createVertexCBuffer(); // For model matrix
	void updateSubResource();

public:
	void draw();
	Model();
	~Model();

	void move(XMVECTOR dPos);
	void rotate(float vx, float vy, float vz, float rotDeg);
	void setRotation(float vx, float vy, float vz, float rotDeg);
	void setRotationAfter(float vx, float vy, float vz, float rotDeg);
	void scale(float xScale, float yScale, float zScale);
	void setPosition(XMVECTOR pos);
	void setPositionRelative(XMVECTOR pos);
	void loadModel(std::ifstream& in);
};

