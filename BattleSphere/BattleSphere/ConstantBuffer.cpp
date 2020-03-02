#include "ConstantBuffer.h"

void ConstantBuffer::createConstantBuffer(const void* data, UINT dataSize)
{
	// Buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = dataSize;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create constant buffer
	HRESULT result = DX::getInstance()->getDevice()->CreateBuffer(&cbDesc, &InitData, &m_constantBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error CreateConstantBuffer", L"Error", MB_OK | MB_ICONERROR);
}

ConstantBuffer::ConstantBuffer(const void* data, UINT dataSize)
{
	createConstantBuffer(data, dataSize);
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_constantBuffer)
		m_constantBuffer->Release();
}

void ConstantBuffer::updateBuffer(const void* data, UINT dataSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	DX::getInstance()->getDeviceContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &data, dataSize);
	DX::getInstance()->getDeviceContext()->Unmap(m_constantBuffer, 0);
}

ID3D11Buffer** ConstantBuffer::getConstantBuffer()
{
	return &m_constantBuffer;
}