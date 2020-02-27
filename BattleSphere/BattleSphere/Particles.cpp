#include "Particles.h"

Particles::Particles()
{
	m_computeShaderParticles = ComputeShader(L"ComputeShaderCulling.hlsl");
	m_vertexShaderParticles = VertexShader(L"VertexShaderCulling.hlsl", 2);
	m_geometryShaderParticles = GeometryShader(L"GeometryShaderCulling.hlsl");
	m_pixelShaderParticles = PixelShader(L"PixelShaderCulling.hlsl");
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
