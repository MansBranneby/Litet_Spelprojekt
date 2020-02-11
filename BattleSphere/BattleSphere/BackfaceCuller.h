#pragma once
#include "VertexShader.h"
#include "GeometryShader.h"
#include "ConstantBuffer.h"
#include "Camera.h"

using namespace DirectX;

class BackfaceCuller
{
private:
	// Shaders
	VertexShader m_vShader;
	GeometryShader m_gShader;

public:
	BackfaceCuller();
	~BackfaceCuller();
};

