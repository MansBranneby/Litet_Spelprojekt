#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
	delete m_constantBufferVP;
	delete m_constantBufferPosition;
}

void Camera::initialize(float width, float height, float nearPlane, float farPlane)
{
	// Base vectors
	m_position = XMVectorSet(0.0f, 70.0f, -60.0f, 0.0f);

	m_lookAt = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);

	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Setup VP
	m_view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
	m_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, nearPlane, farPlane);
	m_viewProjection = XMMatrixMultiply(m_view, m_projection);

	// Create constant buffers
	if (m_constantBufferVP) delete m_constantBufferVP;
	if (m_constantBufferPosition) delete m_constantBufferPosition;

	m_constantBufferVP = new ConstantBuffer(&m_viewProjection, sizeof(m_viewProjection));
	m_constantBufferPosition = new ConstantBuffer(&m_position, sizeof(m_position));
}

XMMATRIX Camera::getViewMatrix()
{
	return m_view;
}

XMMATRIX Camera::getProjectionMatrix()
{
	return m_projection;
}

ConstantBuffer* Camera::getConstantBufferVP()
{
	return m_constantBufferVP;
}

ConstantBuffer* Camera::getConstantBufferPosition()
{
	return m_constantBufferPosition;
}
