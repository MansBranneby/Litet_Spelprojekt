#include "Camera.h"

void Camera::updateBuffers()
{
	// Setup VP
	m_view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
	m_viewProjection = XMMatrixMultiply(m_view, m_projection);

	// Create constant buffers
	delete m_constantBufferVP;
	delete m_constantBufferPosition;
	m_constantBufferVP = new ConstantBuffer(&m_viewProjection, sizeof(m_viewProjection));
	m_constantBufferPosition = new ConstantBuffer(&m_position, sizeof(m_position));
}

Camera::Camera(float width, float height, float nearPlane, float farPlane, bool isPerspective)
{
	if (isPerspective)
	{
		// Base vectors
		m_position = XMVectorSet(0.0f, 65.0f, -70.0f, 0.0f);
	
		m_lookAt = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	
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
	else
	{
		// Base vectors
		m_position = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);

		m_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		// Setup VP
		m_view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
		m_projection = XMMatrixOrthographicLH(width, height, nearPlane, farPlane);
		m_viewProjection = XMMatrixMultiply(m_view, m_projection);

		// Create constant buffers
		m_constantBufferVP = new ConstantBuffer(&m_viewProjection, sizeof(m_viewProjection));
		m_constantBufferPosition = new ConstantBuffer(&m_position, sizeof(m_position));
	}
}

Camera::~Camera()
{
	delete m_constantBufferVP;
	delete m_constantBufferPosition;
}

void Camera::setCameraPosition(float x, float y, float z)
{
	m_position = XMVectorSet(x, y, z, 0.0f);
	updateBuffers();
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
