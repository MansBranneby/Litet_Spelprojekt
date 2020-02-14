#pragma once
#include "SubModel.h"
#include <string>
#include <fstream>
#include <sstream>


class Model
{
private:
	XMVECTOR m_pos;
	XMVECTOR m_relPos;
	int m_nrOfVertices;
	vertex* m_vertices;
	int m_nrOfSubModels;
	SubModel* m_subModels;

	XMMATRIX m_modelMatrix;

	ID3D11Buffer* m_matrixCBuffer;
	XMMATRIX* m_matrixData;
	XMMATRIX m_rotationMat;
	XMMATRIX m_rotationAfterMat;
	XMMATRIX m_scalingMat;
	XMMATRIX m_relRotationMat;
	XMMATRIX m_relScalingMat;
	ID3D11Buffer* m_vertexBuffer;

	void createVertexBuffer(); // For vertex buffer
	void createVertexCullingBuffer(); // For vertex culling buffer
	void createVertexCBuffer(); // For model matrix
	void updateSubResource();
	void updateRelSubResource();

    void setPosition(XMVECTOR pos);
	void setPosition(XMVECTOR pos, XMVECTOR relPos);
	void setRotation(XMVECTOR rotation);
	void setRotation(XMVECTOR rotation, XMVECTOR relRotation);
	void setScale(XMVECTOR scale);
	void setScale(XMVECTOR scale, XMVECTOR relScale);

	// For backface culling
	ID3D11Buffer* m_vertexCullingBuffer;
	vertexAndId* m_vertexAndId;
	void setVPMatrix();

public:
	void draw();
	void cullDraw();
	Model();
	~Model();

	void setObjectData(objectData data, int modelNr = -1);
	void setObjectData(objectData data, objectData relativeData, int modelNr = -1);
	void loadModel(std::ifstream& in);
};

