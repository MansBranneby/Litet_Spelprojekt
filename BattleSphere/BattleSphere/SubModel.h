#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

struct material 
{
	XMVECTOR ambient; // ambient.xyz, illumination model enumeration (illum)
	XMVECTOR diffuse; // diffuse.xyz, refraction (Ni)
	XMVECTOR specular; // specular.xyz, shininess (Ns)
	XMVECTOR emission; // emission.xyz, opacity (d)
};

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
	void createCulledIndexBuffer();

	// Backface culling
	ID3D11Buffer* m_culledIndiceBuffer;
	int m_nrOfCulledIndices;
	int* m_culledIndices;

public:
	SubModel();
	~SubModel();

	void setMaterialInfo(material mat);
	void setFaces(int* indexBuffer, int nrOfIndices);

	void draw();
	void cullDraw();
};

