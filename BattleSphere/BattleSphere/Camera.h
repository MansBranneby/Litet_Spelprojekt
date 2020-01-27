#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "ConstantBuffer.h"

using namespace DirectX;

class Camera
{
private:
	// Vectors
	XMVECTOR m_position;

	XMVECTOR m_lookAt;
	XMVECTOR m_up;
	XMVECTOR m_right;
	XMVECTOR m_forward;

	// Matrices
	XMMATRIX m_view;
	XMMATRIX m_projection;
	XMMATRIX m_viewProjection;

	// Constant buffer
	ConstantBuffer *m_constantBufferVP = nullptr;

public:
	Camera(float width, float height, float nearPlane, float farPlane);
	~Camera();

	ConstantBuffer* getConstantBufferVP();
};

