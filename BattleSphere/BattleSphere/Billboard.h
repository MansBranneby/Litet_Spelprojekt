#pragma once
#include <DirectXMath.h>
#include "VertexShader.h"
#include "StructsAndEnums.h"

// Class used to animate billboards
class Billboard
{
private:
	// Connection to subModelNr
	int m_variant;
	int m_modelNr;
	int m_subModelNr;

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

	// EFFECTS
	void moveUV(float dt); // Translation of texture
	void changeColour(float dt); // Interpolate between colours;

public:
	Billboard();
	Billboard(int variant, int modelNr, int subModelNr);
	Billboard(DirectX::XMVECTOR velocityUV, float blinkSpeed, float colourChangeSpeed, float type, int variant, int modelNr, int subModelNr);

	int getVariant() const;
	int getModelNr() const;
	int getSubModelNumber() const;
	DirectX::XMVECTOR getVelocityUV() const;
	BillboardData getBillboardData() const;

	void update(float dt);

};