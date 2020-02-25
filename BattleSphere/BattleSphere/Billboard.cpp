#include "Billboard.h"

Billboard::Billboard()
{
	m_velocityUV = { 0.0f, 0.0f, 0.0f };
}

void Billboard::setVelocityUV(DirectX::XMVECTOR velocityUV)
{
	if (m_velocityUV.m128_f32[0] < -1.0f)
		m_velocityUV.m128_f32[0] = 1.0f;
	else
		m_velocityUV = velocityUV;

}

DirectX::XMVECTOR Billboard::getVelocityUV() const
{
	return m_velocityUV;
}

TextureAnimationData Billboard::getTextureAnimationData() const
{
	TextureAnimationData data;
	data.velocityUV = m_velocityUV;

	return data;
}
