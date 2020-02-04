#include "Camera.h"

Camera::Camera(float width, float height, float nearPlane, float farPlane)
{
	// Base vectors
	m_position = XMVectorSet(0.0f, 45.0f, -60.0f, 0.0f);
	
	m_lookAt = XMVectorSet(0.0f, 0.0f, -23.0f, 0.0f);
	
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Setup VP
	m_view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
	m_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, nearPlane, farPlane);
	m_viewProjection = XMMatrixMultiply(m_view, m_projection);

	// Create constant buffers
	m_constantBufferVP = new ConstantBuffer(&m_viewProjection, sizeof(m_viewProjection));
	m_constantBufferPosition = new ConstantBuffer(&m_position, sizeof(m_position));
}

Camera::~Camera()
{
	delete m_constantBufferVP;
	delete m_constantBufferPosition;
}

ConstantBuffer* Camera::getConstantBufferVP()
{
	return m_constantBufferVP;
}

ConstantBuffer* Camera::getConstantBufferPosition()
{
	return m_constantBufferPosition;
}
