#pragma once

#include <DirectXMath.h>

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

	// Constant buffers
	ConstantBuffer* m_constantBufferVP = nullptr;
	ConstantBuffer* m_constantBufferPosition = nullptr;

public:
	Camera(float width, float height, float nearPlane, float farPlane);
	~Camera();

	ConstantBuffer* getConstantBufferVP();
	ConstantBuffer* getConstantBufferPosition();
};

