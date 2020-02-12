#include "BackfaceCuller.h"

void BackfaceCuller::turnOnCullingPipeline()
{
	DX::getInstance()->getDeviceContext()->VSSetShader(&m_vShader.getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(&m_gShader.getGeometryShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

void BackfaceCuller::turnOffCullingPipeline()
{
	DX::getInstance()->getDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

BackfaceCuller::BackfaceCuller()
{
	m_vShader = VertexShader(L"VertexShaderCulling.hlsl", 1);
	m_gShader = GeometryShader(L"GeometryShaderCulling.hlsl", 1);
}

BackfaceCuller::~BackfaceCuller()
{
	// Release shaders
	m_vShader.release();
	m_gShader.release();
}
