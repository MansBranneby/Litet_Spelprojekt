#include "Particles.h"

Particles::Particles()
{
	// Create shaders
	m_computeShaderParticles = ComputeShader(L"ComputeShaderParticles.hlsl");
	m_vertexShaderParticles = VertexShader(L"VertexShaderParticles.hlsl", 2);
	m_geometryShaderParticles = GeometryShader(L"GeometryShaderParticles.hlsl");
	m_pixelShaderParticles = PixelShader(L"PixelShaderParticles.hlsl");

	// Set prev and updated pointers to definite particle buffers
	m_prevParticles = &m_actualBuffers[0];
	m_updatedParticles = &m_actualBuffers[1];

	// Create compute shader particle structured buffers and unordered access views
	D3D11_BUFFER_DESC sBDesc = {};
	sBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_UNORDERED_ACCESS;
	sBDesc.Usage = D3D11_USAGE_DYNAMIC;
	sBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sBDesc.StructureByteStride = sizeof(particle);
	sBDesc.ByteWidth = sizeof(particle) * MAX_PARTICLES;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = MAX_PARTICLES;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = MAX_PARTICLES;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sBDesc, 0, &m_prevParticles->sBParticles)))
	{
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_prevParticles->sBParticles, &uavDesc, &m_prevParticles->sBParticlesUAV);
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_prevParticles->sBParticles, &srvDesc, &m_prevParticles->sBParticlesSRV);
	}

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sBDesc, 0, &m_updatedParticles->sBParticles)))
	{
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_updatedParticles->sBParticles, &uavDesc, &m_updatedParticles->sBParticlesUAV);
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_updatedParticles->sBParticles, &srvDesc, &m_updatedParticles->sBParticlesSRV);
	}

	// Create compute shader particle param structured buffers and unordered access views
	D3D11_BUFFER_DESC paramSBDesc = {};
	paramSBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	paramSBDesc.Usage = D3D11_USAGE_DYNAMIC;
	paramSBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	paramSBDesc.StructureByteStride = sizeof(particleDispatchParams);
	paramSBDesc.ByteWidth = sizeof(particleDispatchParams);
	paramSBDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	uavDesc.Format = DXGI_FORMAT_R32_UINT;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = sizeof(particleDispatchParams) / sizeof(UINT);

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sBDesc, 0, &m_prevParticles->sBParams)))
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_prevParticles->sBParams, &uavDesc, &m_prevParticles->sBParamsUAV);

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sBDesc, 0, &m_updatedParticles->sBParams)))
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_updatedParticles->sBParams, &uavDesc, &m_updatedParticles->sBParamsUAV);


	// Create compute shader constant buffer, to update delta time
	m_computeShaderConstantBuffer = new ConstantBuffer(&XMVectorSet(0, 0, 0, 0), sizeof(XMVECTOR));

	// TODO: Remove
	particle test =
	{
		0, 5, 0,
		0, 5, 0,
		20, 20,
		0, 1, 0
	};
	m_particleToAdd.push_back(test);
}

Particles::~Particles()
{
	// Shaders
	m_computeShaderParticles.release();
	m_vertexShaderParticles.release();
	m_geometryShaderParticles.release();
	m_pixelShaderParticles.release();

	// Buffers and views
	// TODO: Fix
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
	
	// Reset updated param buffer and add new particle params to loop
	particleDispatchParams params;
	params.drawParams.vertexCountPerInstance = (UINT)m_particleToAdd.size();
	params.threadGroupCount = (UINT)1 + (UINT)m_particleToAdd.size() / (UINT)64;
	DX::getInstance()->getDeviceContext()->Map(m_updatedParticles->sBParams, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &params, sizeof(particleDispatchParams));
	DX::getInstance()->getDeviceContext()->Unmap(m_updatedParticles->sBParams, 0);
	
	// Reset updated particle buffer and add new particles to loop
	DX::getInstance()->getDeviceContext()->Map(m_updatedParticles->sBParticles, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_particleToAdd.data(), sizeof(newParticleGroup));
	DX::getInstance()->getDeviceContext()->Unmap(m_updatedParticles->sBParticles, 0);
	
	// Clear particles to add
	m_particleToAdd.clear();
	//
	//// Set unordered access views, read from prev and write to updated
	//ID3D11UnorderedAccessView* uAV[4] =
	//{
	//	m_prevParticles->sBParamsUAV,
	//	m_prevParticles->sBParticlesUAV,
	//	m_updatedParticles->sBParamsUAV,
	//	m_updatedParticles->sBParticlesUAV
	//};
	//DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, _countof(uAV), uAV, 0);
	//
	//// Bind constant buffer
	//DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(0, 1, m_computeShaderConstantBuffer->getConstantBuffer());
	//
	//// Run compute shader
	//DX::getInstance()->getDeviceContext()->DispatchIndirect(m_prevParticles->sBParams, sizeof(drawParameters));
	//
	//// Unbind constant buffer
	//DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(0, 1, nullptr);
	//
	//// Unbind UAVs
	//ID3D11UnorderedAccessView* emptyUAVs[4] =
	//{
	//	nullptr,
	//	nullptr,
	//	nullptr,
	//	nullptr
	//};
	//DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, _countof(emptyUAVs), emptyUAVs, 0);
	//
	//// Swap prev and updated structured buffer order
	//std::swap(m_prevParticles, m_updatedParticles);
}

void Particles::draw()
{
	// Enable emission
	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSEnabled(), 1);

	// Disable unused shaders
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);

	// Set input assembler
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	DX::getInstance()->getDeviceContext()->IASetInputLayout(&m_vertexShaderParticles.getvertexLayout());

	// Set vertex shader
	DX::getInstance()->getDeviceContext()->VSSetShader(&m_vertexShaderParticles.getVertexShader(), nullptr, NULL);
	DX::getInstance()->getDeviceContext()->VSGetShaderResources(0, 1, &m_updatedParticles->sBParticlesSRV);

	// Set geometry shader
	ID3D11Buffer* camInfo[2] =
	{
		*DX::getInstance()->getCam()->getConstantBufferPosition()->getConstantBuffer(),
		*DX::getInstance()->getCam()->getConstantBufferVP()->getConstantBuffer()
	};
	DX::getInstance()->getDeviceContext()->GSSetConstantBuffers(0, 2, camInfo);
	DX::getInstance()->getDeviceContext()->GSSetShader(&m_geometryShaderParticles.getGeometryShader(), nullptr, NULL);

	// Set pixel shader
	DX::getInstance()->getDeviceContext()->PSSetShader(&m_pixelShaderParticles.getPixelShader(), nullptr, NULL);

	// Draw
	DX::getInstance()->getDeviceContext()->DrawInstancedIndirect(m_updatedParticles->sBParams, 0); // Read and updated from ping, draw from pong

	// Disable emission
	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);

	// Disable used shaders
	DX::getInstance()->getDeviceContext()->VSGetShaderResources(0, 1, nullptr);
	DX::getInstance()->getDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSGetShaderResources(0, 1, nullptr);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}
