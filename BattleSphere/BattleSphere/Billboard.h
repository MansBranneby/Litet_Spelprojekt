#pragma once
#include <DirectXMath.h>
#include "VertexShader.h"
#include "StructsAndEnums.h"

// Class used to animate textures
class Billboard
{
private:
	DirectX::XMVECTOR m_velocityUV;

public:
	Billboard();

	void setVelocityUV(DirectX::XMVECTOR velocityUV);
	DirectX::XMVECTOR getVelocityUV() const;

	TextureAnimationData getTextureAnimationData() const;
};