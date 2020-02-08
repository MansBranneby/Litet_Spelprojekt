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
	XMVECTOR m_relPos;
	int m_nrOfVertices;
	vertex* m_vertices;
	int m_nrOfSubModels;
	SubModel* m_subModels;
	
	// Bounding volume variables
	boundingData m_bData;

	std::vector<int> m_indices;


	ID3D11Buffer* m_modelMatrixCBuffer;
	XMMATRIX* m_modelMatrixData;
	XMMATRIX m_rotationMat;
	XMMATRIX m_rotationAfterMat;
	XMMATRIX m_scalingMat;
	XMMATRIX m_relRotationMat;
	XMMATRIX m_relScalingMat;
	ID3D11Buffer* m_vertexBuffer;

	// To calculate obb
	void computeOBB();

	void createVertexBuffer(); // For vertex buffer
	void createVertexCBuffer(); // For model matrix
	void updateSubResource();
	void updateRelSubResource();

    void setPosition(XMVECTOR pos);
	void setPosition(XMVECTOR pos, XMVECTOR relPos);
	void setRotation(XMVECTOR rotation);
	void setRotation(XMVECTOR rotation, XMVECTOR relRotation);
	void setScale(XMVECTOR scale);
	void setScale(XMVECTOR scale, XMVECTOR relScale);
public:
	void draw();
	Model();
	~Model();

	objectData getBVObjectData() const;
	boundingData getBoundingData() const;
	std::vector<XMFLOAT3> getCollisionMesh(objectData data); // Delete after use
	std::vector<XMFLOAT3> getCollisionMesh(objectData data, objectData relativeData); // Delete after use
	void setObjectData(objectData data);
	void setObjectData(objectData data, objectData relativeData);
	void loadModel(std::ifstream& in, bool isCollisionMesh = false);
};

