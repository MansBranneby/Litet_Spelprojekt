#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "DX.h"


class ConstantBuffer
{
private:
	ID3D11Buffer* m_constantBuffer = nullptr;
	void createConstantBuffer(const void* data, UINT dataSize);

public:
	ConstantBuffer(const void* data, UINT dataSize);
	~ConstantBuffer();

	void updateBuffer(const void* data, UINT dataSize);
	ID3D11Buffer** getConstantBuffer();
};