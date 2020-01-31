#pragma once
#include "SubModel.h"
#include <string>
#include <fstream>
#include <sstream>
#include "StructsAndEnums.h"


class Model
{
private:
	XMVECTOR m_pos;
	int m_nrOfVertices;
	vertex* m_vertices;
	int m_nrOfSubModels;
	SubModel* m_subModels;

	XMMATRIX m_modelMatrix;

	ID3D11Buffer* m_modelMatrixCBuffer;
	XMMATRIX* m_modelMatrixData;
	XMMATRIX m_rotationMat;
	XMMATRIX m_scalingMat;
	ID3D11Buffer* m_vertexBuffer;

	void createVertexBuffer(); // For vertex buffer
	void createVertexCBuffer(); // For model matrix
	void updateSubResource();

    void setPosition(XMVECTOR pos);
	void setRotation(XMVECTOR rotation);
	void setScale(XMVECTOR scale);
public:
	void draw();
	Model();
	~Model();

	void setObjectData(objectData data);
	void loadModel(std::ifstream& in);
};

