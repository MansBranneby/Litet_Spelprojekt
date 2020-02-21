#include "Camera.h"

/*
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
*/

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
		if (m_constantBufferVP) delete m_constantBufferVP;
		if (m_constantBufferPosition) delete m_constantBufferPosition;

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

Camera::Camera()
{
	// Base vectors
	m_position = XMVectorSet(0.0f, 70.0f, -60.0f, 0.0f);

	m_lookAt = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);

	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Setup VP
	m_view = XMMatrixIdentity();
	m_projection = XMMatrixIdentity();
	m_viewProjection = XMMatrixIdentity();

	// DX
	m_constantBufferVP = nullptr;
	m_constantBufferPosition = nullptr;
}

void Camera::initialize(float width, float height, float nearPlane, float farPlane)
{
	// Base vectors
	m_position = XMVectorSet(0.0f, 120.0f, -110.0f, 0.0f);

	m_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

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

Camera::Camera(float width, float height, float nearPlane, float farPlane)
{
	// Base vectors
	m_position = XMVectorSet(0.0f, 90.0f, -40.0f, 0.0f);

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

Camera::Camera(float width, float height, float nearPlane, float farPlane, XMVECTOR pos)
{
	// Base vectors
	m_position = pos;

	m_lookAt = XMVectorSet(10.0f, 0.0f, 15.0f, 0.0f);

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
	if (m_constantBufferVP) delete m_constantBufferVP;
	if (m_constantBufferPosition) delete m_constantBufferPosition;
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

void Camera::updateBuffers()
{
	m_view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);
	m_viewProjection = XMMatrixMultiply(m_view, m_projection);

	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferVP->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &m_viewProjection, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferVP->getConstantBuffer(), 0);

	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferPosition->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &m_position, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferPosition->getConstantBuffer(), 0);
}

XMVECTOR Camera::getPosition()
{
	return m_position;
}

void Camera::setPosition(XMVECTOR pos)
{
	m_position = pos;
}
