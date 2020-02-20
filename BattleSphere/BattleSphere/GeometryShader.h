#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DX.h"

class GeometryShader
{
private:
	ID3D11GeometryShader* m_geometryShader;

	// Creates VertexBuffer, this is called upon in the Constructor
	HRESULT createGeometryShader(LPCWSTR fileName, ID3DBlob** pGS, ID3DBlob** errorBlob);
	HRESULT createSOGeometryShader(LPCWSTR fileName, ID3DBlob** pGS, ID3DBlob** errorBlob); // Stream Output


public:
	GeometryShader();
	GeometryShader(LPCWSTR fileName, int type = 0); // 0: Standard, 1: Stream Output

	ID3D11GeometryShader& getGeometryShader() const;

	void release();
};

