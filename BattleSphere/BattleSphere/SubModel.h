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

	// Texture
	ID3D11ShaderResourceView* m_SRV;
	ConstantBuffer* m_constantBufferBillboard;

	// Billboard data
	BillboardData m_billboardData;

public:
	SubModel();
	~SubModel();

	void setBillboardData(BillboardData billboardData);
	void updateTextureAnimationCB(BillboardData billboardData);
	void setMaterialInfo(material mat);
	void updateMaterialInfo(material mat);
	void setFaces(int* indexBuffer, int nrOfIndices);
	void setSRV(ID3D11ShaderResourceView* SRV);

	BillboardData getBillboardData();
	
	void draw();
	void draw(BillboardData billboardData);
	void cullDraw();
};

