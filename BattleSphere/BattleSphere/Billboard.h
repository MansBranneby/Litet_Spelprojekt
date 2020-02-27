#pragma once
#include <DirectXMath.h>
#include "VertexShader.h"
#include "StructsAndEnums.h"

// Class used to animate billboards
class Billboard
{
private:
	// TYPE OF BILLBOARD
	float m_type; // Indicates what type this billboard is, eg blinking etc
	// TRANSLATE TEXTURE
	DirectX::XMVECTOR m_velocityUV; // Velocity of UV coordinates
	DirectX::XMVECTOR m_UVincrement; // Used to increment texture UV coordinates
	// FADE COLOUR
	float m_blinkSpeed; // Determines how fast the colour fades in and out
	float m_blinkFactor; // Value 0-1, it ticks up and down
	// INTERPOLATE COLOURS
	float m_colourChangeSpeed; // Determines how fast colour changes
	float m_colourChangeFactor; // Value 0-1, it resets to 0
	float m_colourDecider; // Flips the interpolated colours inside function ChangeColour in PixelShaderBillboard.hlsl
	

public:
	Billboard();
	Billboard(DirectX::XMVECTOR velocityUV, float blinkSpeed, float colourChangeSpeed, float type);

	// Calculates m_UVincrement
	void moveUV(float dt);
	void changeColour(float dt);

	DirectX::XMVECTOR getVelocityUV() const;
	TextureAnimationData getTextureAnimationData() const;
};