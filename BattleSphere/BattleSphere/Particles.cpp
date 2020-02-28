#include "Particles.h"

Particles::Particles()
{
	// Create shaders
	m_computeShaderParticles = ComputeShader(L"ComputeShaderCulling.hlsl");
	m_vertexShaderParticles = VertexShader(L"VertexShaderCulling.hlsl", 2);
	m_geometryShaderParticles = GeometryShader(L"GeometryShaderCulling.hlsl");
	m_pixelShaderParticles = PixelShader(L"PixelShaderCulling.hlsl");

	// Create compute shader structured buffers
	m_structuredBufferCSPing = nullptr;
	m_structuredBufferCSPong = nullptr;
	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = sizeof(particle);
	sbDesc.ByteWidth = sizeof(particle) * MAX_PARTICLES;


	// Create compute shader unordered access views
	m_structuredBufferUAVCS[0] = nullptr;
	m_structuredBufferUAVCS[1] = nullptr;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = MAX_PARTICLES; // Including instanced draw parameters
	uavDesc.Buffer.Flags = 0;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sbDesc, 0, &m_structuredBufferCSPing)))
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_structuredBufferCSPing, &uavDesc, &m_structuredBufferUAVCS[0]);

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sbDesc, 0, &m_structuredBufferCSPong)))
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_structuredBufferCSPong, &uavDesc, &m_structuredBufferUAVCS[1]);
	m_readIndex = 0;

	// Create compute shader constant buffer, to update delta time
	m_computeShaderConstantBuffer = new ConstantBuffer(nullptr, sizeof(XMVECTOR));
}

Particles::~Particles()
{
	m_computeShaderParticles.release();
	m_vertexShaderParticles.release();
	m_geometryShaderParticles.release();
	m_pixelShaderParticles.release();
}

void Particles::addParticles(XMVECTOR position, XMVECTOR color, XMVECTOR size, int amount, float velocity, XMVECTOR direction)
{

}

void Particles::update(float dT)
{
	// Update compute shader constant buffer with delta time
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	ID3D11Buffer* tempPtr = *m_computeShaderConstantBuffer->getConstantBuffer();
	DX::getInstance()->getDeviceContext()->Map(tempPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(dT, dT, dT, dT), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(tempPtr, 0);
}

void Particles::draw()
{
	// Set unordered access views, read from 0 and write to 1
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 2, m_structuredBufferUAVCS, 0);

	// Run compute shader
	DX::getInstance()->getDeviceContext()->Dispatch(16, 16, 2); // 512 groups


	//// Enable emission
	//DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSEnabled(), 1);
	//
	// Disable unused shaders
	//DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	//DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	//// Set input assembler
	//DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//DX::getInstance()->getDeviceContext()->IASetInputLayout(&m_vertexShaderParticles.getvertexLayout());
	//
	//// Set vertex shader
	//DX::getInstance()->getDeviceContext()->VSSetShader(&m_vertexShaderParticles.getVertexShader(), nullptr, NULL);
	//ID3D11Buffer* vPCameraCBuffer = *DX::getInstance()->getCam()->getConstantBufferVP()->getConstantBuffer();
	//DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, &vPCameraCBuffer);
	//
	//// Set geometry shader
	//DX::getInstance()->getDeviceContext()->GSSetShader(&m_geometryShaderParticles.getGeometryShader(), nullptr, NULL);
	//
	//// Set pixel shader
	//DX::getInstance()->getDeviceContext()->PSSetShader(&m_pixelShaderParticles.getPixelShader(), nullptr, NULL);
	//
	//// Draw
	//if (m_readIndex == 0)
	//	DX::getInstance()->getDeviceContext()->DrawInstancedIndirect(m_structuredBufferCSPong, 0); // Read and updated from ping, draw from pong
	//else
	//	DX::getInstance()->getDeviceContext()->DrawInstancedIndirect(m_structuredBufferCSPing, 0); // Read and updated from pong, draw from ping
	//
	//// Disable emission
	//DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);
	//
	//// Disable used shaders
	//DX::getInstance()->getDeviceContext()->VSSetShader(nullptr, nullptr, NULL);
	//DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, NULL);
	//DX::getInstance()->getDeviceContext()->PSSetShader(nullptr, nullptr, NULL);
	//
	//// Swap ping and pong structured buffer order
	//std::swap(m_structuredBufferUAVCS[0], m_structuredBufferUAVCS[1]);
	//m_readIndex = (m_readIndex + 1) % 2; // [0, 1]
}
