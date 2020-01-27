#include "Camera.h"

Camera::Camera(float width, float height, float nearPlane, float farPlane)
{
	// TODO: delete comments

	// Base vectors
	m_position = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
	
	m_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Setup VP
	m_view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
	//m_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, 0.1f, 200.0f);
	m_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, nearPlane, farPlane);
	m_view = XMMatrixTranspose(m_view);
	m_projection = XMMatrixTranspose(m_projection);
	m_viewProjection = XMMatrixMultiply(m_projection, m_view);

	// Create constant buffer for VP
	m_constantBufferVP = new ConstantBuffer(&m_viewProjection, sizeof(m_viewProjection));
}

Camera::~Camera()
{
	delete m_constantBufferVP;
}

ConstantBuffer* Camera::getConstantBufferVP()
{
	return m_constantBufferVP;
}
