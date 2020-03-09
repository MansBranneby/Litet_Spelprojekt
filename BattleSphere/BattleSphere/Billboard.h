#pragma once
#include <DirectXMath.h>
#include "VertexShader.h"
#include "StructsAndEnums.h"
#include "GameObject.h"

// Class used to animate billboards
class Billboard : public GameObject
{
private:
	// TYPE OF BILLBOARD
	BillboardState m_state; // Indicates what type this billboard is, eg flasing, interpolating or translating (FIT)

	// Billboard knows its own objectType, variant, modelNr and submodelNr
	ObjectType m_objectType;
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
	DirectX::XMVECTOR m_colourA; // interpolate between colourA and B
	DirectX::XMVECTOR m_colourB; // interpolate between colourA and B

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
	Billboard(ObjectType objectType, int variant, int modelNr, int subModelNr, BillboardData billboardData);

	void setState(BillboardState state); 
	void setFlashState(float flashSpeed); // Initialize flash state
	void setInterpolateState(float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB); // Initialize interpolate state
	void setTranslateState(DirectX::XMVECTOR velocityUV); // Initialize translate state
	void setFlashInterpolateState(float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB); // Initialize flash and interpolate state
	void setFlashTranslateState(float flashSpeed, DirectX::XMVECTOR velocityUV); // Initialize flash and translate state
	void setInterpolateTranslateState(float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV); // Initialize interpolate and translate state
	void setAllStates(float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV); // initialize flash, interpolate and translate state
	void setFlashSpeed(float flashSpeed); 
	void setColourChangeSpeed(float colourChangeSpeed);
	void setVelocityUV(DirectX::XMVECTOR velocityUV);

	ObjectType getObjectType() const;
	int getVariant() const;
	int getModelNr() const;
	int getSubModelNumber() const;
	BillboardData getBillboardData() const;

	void update(float dt);
};