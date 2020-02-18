#include "Transparency.h"


void Transparency::initialize()
{
	m_info = new robotInfo();
	for (int i = 0; i < 4; i++)
	{
		m_info->position[0] = XMFLOAT4(0, 0, 0,0);
	}
	
	m_constantBuffer = new ConstantBuffer(m_info, sizeof(robotInfo));
}

void Transparency::update(XMVECTOR position, XMMATRIX viewMatrix, XMMATRIX projMatrix, int index)
{
	XMVECTOR edge = XMVectorSet(position.m128_f32[0] + 1, position.m128_f32[1], position.m128_f32[2], 1);
	position = XMVector4Transform(position, viewMatrix);
	position = XMVector4Transform(position, projMatrix);
	position = XMVectorSet((position.m128_f32[0] / position.m128_f32[3]), (position.m128_f32[1] / position.m128_f32[3]), position.m128_f32[2] / position.m128_f32[3], 1.0);
	
	edge = XMVector4Transform(edge, viewMatrix);
	edge = XMVector4Transform(edge, projMatrix);
	edge = XMVectorSet((edge.m128_f32[0] / edge.m128_f32[3]), (edge.m128_f32[1] / edge.m128_f32[3]), edge.m128_f32[2] / edge.m128_f32[3], 1.0);
	float radius = edge.m128_f32[0] - position.m128_f32[0];
	if (radius < 0)
		radius *= -1;
	m_info->position[index].x = position.m128_f32[0];
	m_info->position[index].y = position.m128_f32[1];
	m_info->position[index].z = position.m128_f32[2];
	m_info->position[index].w = radius;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(*m_constantBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_info, sizeof(robotInfo));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBuffer->getConstantBuffer(), 0);

}

void Transparency::bindConstantBuffer()
{
	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(4, 1, m_constantBuffer->getConstantBuffer());
}

Transparency::Transparency()
{
	m_info = nullptr;
	m_constantBuffer = nullptr;

}

Transparency::~Transparency()
{
	delete m_info;
	delete m_constantBuffer;
}
