#pragma once

#include <DirectXMath.h>

#include "ConstantBuffer.h"

using namespace DirectX;

class DX; // Forward declaration
class ConstantBuffer; // Do not touch sacred ground

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
	ConstantBuffer* m_constantBufferVP;
	ConstantBuffer* m_constantBufferPosition;

	void updateBuffers();

public:
	Camera();
	Camera(float width, float height, float nearPlane, float farPlane);
	Camera(float width, float height, float nearPlane, float farPlane, bool isPerspective = true);
	Camera(float width, float height, float nearPlane, float farPlane, XMVECTOR pos);
	~Camera();
	void initialize(float width, float height, float nearPlane, float farPlane);

	void setCameraPosition(float x, float y, float z);

	XMMATRIX getViewMatrix();
	XMMATRIX getProjectionMatrix();
	ConstantBuffer* getConstantBufferVP();
	ConstantBuffer* getConstantBufferPosition();
};

