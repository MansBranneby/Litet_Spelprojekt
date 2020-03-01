#pragma once
#include <DirectXMath.h>
#include "VertexShader.h"
#include "StructsAndEnums.h"

// Class used to animate billboards
class Billboard
{
private:
	// TYPE OF BILLBOARD
	BillboardState m_state; // Indicates what type this billboard is, eg flasing, interpolating or translating (FIT)

	// Billboard knows its own variant, modelNr and submodelNr
	int m_variant;
	int m_modelNr;
	int m_subModelNr;

	// FLASH COLOUR
	float m_flashSpeed; // Determines how fast the colour fades in and out
	float m_flashFactor; // Value 0-1, it ticks up and down

	// INTERPOLATE COLOURS
	float m_colourChangeSpeed; // Determines how fast colour changes
	float m_colourChangeFactor; // Value 0-1, ticks up to 1 and resets to 0
	float m_colourDecider; // Flips the interpolated colours inside function ChangeColour in PixelShaderBillboard.hlsl

	// TRANSLATE TEXTURE
	DirectX::XMVECTOR m_velocityUV; // Velocity of UV coordinates
	DirectX::XMVECTOR m_UVincrement; // Used to increment texture UV coordinates

	// EFFECTS
	void flashColour(float dt);
	void interpolateColours(float dt); // Interpolate between colours;
	void translateUV(float dt); // Translation of texture

public:
	Billboard();
	Billboard(int variant, int modelNr, int subModelNr);

	void setType(BillboardState state);
	void setFlashSpeed(float flashSpeed);
	void setColourChangeSpeed(float colourChangeSpeed);
	void setVelocityUV(DirectX::XMVECTOR velocityUV);

	int getVariant() const;
	int getModelNr() const;
	int getSubModelNumber() const;
	BillboardData getBillboardData() const;

	void update(float dt);
};