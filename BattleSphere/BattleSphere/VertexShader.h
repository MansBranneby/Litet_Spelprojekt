#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DX.h"

class VertexShader
{
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_vertexLayout;

	// Creates standard VertexBuffer, this is called upon in the Constructor
	HRESULT createVertexShader(LPCWSTR fileName, ID3DBlob** pVS, ID3DBlob** errorBlob);

	// Creates standard input layout
	void createInputLayout(ID3DBlob** pVS, ID3DBlob** errorBlob);

	// Creates backface culling input layout
	void createBCInputLayout(ID3DBlob** pVS, ID3DBlob** errorBlob);

	// Create particle input layout
	void createParticleInputLayout(ID3DBlob** pVS, ID3DBlob** errorBlob);

public:
	VertexShader();
	VertexShader(LPCWSTR fileName, int type = 0);

	ID3D11VertexShader& getVertexShader() const;
	ID3D11InputLayout& getvertexLayout() const;

	void release();
};