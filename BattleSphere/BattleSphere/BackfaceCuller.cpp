#include "BackfaceCuller.h"

BackfaceCuller::BackfaceCuller()
{
	m_vShader = VertexShader(L"VertexShaderCulling.hlsl", 1);
	m_gShader = GeometryShader(L"GeometryShaderCulling.hlsl");
}

BackfaceCuller::~BackfaceCuller()
{
	// Release shaders
	m_vShader.release();
	m_gShader.release();
}
