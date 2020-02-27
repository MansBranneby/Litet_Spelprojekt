#include "Particles.h"

Particles::Particles()
{
	// Create shaders
	m_computeShaderParticles = ComputeShader(L"ComputeShaderCulling.hlsl");
	m_vertexShaderParticles = VertexShader(L"VertexShaderCulling.hlsl", 2);
	m_geometryShaderParticles = GeometryShader(L"GeometryShaderCulling.hlsl");
	m_pixelShaderParticles = PixelShader(L"PixelShaderCulling.hlsl");

	// Create compute shader structured buffer and unordered access view
	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	sbDesc.CPUAccessFlags = 0;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = sizeof(particle);
	sbDesc.ByteWidth = sizeof(particle) * MAX_PARTICLES;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	DX::getInstance()->getDevice()->CreateBuffer(&sbDesc, 0, &m_structuredBufferCSWrite);

	this->m_directX->getDevice()->CreateBuffer(&sbDesc, nullptr, &this->m_structuredBufferIn);


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = PARTICLE_COUNT;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	this->m_directX->getDevice()->CreateUnorderedAccessView(this->m_structuredBufferOut, &uavDesc, &this->m_structuredBufferUAVOut);


	// Create vertex shader shader resource view and constant buffer

	// Create pixel shader constant buffer
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
}

void Particles::draw()
{
	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSEnabled(), 1);


	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);
}
