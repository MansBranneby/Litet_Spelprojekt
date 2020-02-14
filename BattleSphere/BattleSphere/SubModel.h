#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "StructsAndEnums.h"

using namespace DirectX;

class SubModel
{
private:
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_materialCBuffer;
	ConstantBuffer* m_constantBuffer;
	material* m_mat = nullptr;
	int m_nrOfIndices;
	int* m_indexArray;

	void createIndexBuffer();
	void createCulledIndexBuffer(); // Create culled index buffer and it's clear buffer

	// Backface culling
	ID3D11Buffer* m_culledIndiceBuffer;
	ID3D11Buffer* m_clearedIndiceBuffer;
	int m_nrOfCulledIndices;

public:
	SubModel();
	~SubModel();

	void setMaterialInfo(material mat);
	void updateMaterialInfo(material mat);
	void setFaces(int* indexBuffer, int nrOfIndices);

	void draw();
	void cullDraw();
};

