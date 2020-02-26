#pragma once
#include <DirectXMath.h>
#include "VertexShader.h"
#include "StructsAndEnums.h"

// Class used to animate billboards
class Billboard
{
private:
	DirectX::XMVECTOR m_velocityUV; // Velocity of UV coordinates
	DirectX::XMVECTOR m_UVincrement; // Used to increment texture UV coordinates
	float m_blinkSpeed; // Determines how fast the alpha value changes
	float m_blinkValue; //
	float m_type;
public:
	Billboard();
	Billboard(DirectX::XMVECTOR velocityUV, float blinkSpeed, float type);

	// Calculates m_UVincrement
	void moveUV(float dt);

	DirectX::XMVECTOR getVelocityUV() const;
	TextureAnimationData getTextureAnimationData() const;
};