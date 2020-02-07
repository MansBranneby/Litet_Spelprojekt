#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DX.h"

class ComputeShader
{
private:
	ID3D11ComputeShader* m_computeShader;

	HRESULT createComputeShader(LPCWSTR fileName, ID3DBlob** pVS, ID3DBlob** errorBlob);

public:
	
	ComputeShader();
	ComputeShader(LPCWSTR fileName);

	ID3D11ComputeShader& getComputeShader() const;

	void release();
};