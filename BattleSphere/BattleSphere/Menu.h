#pragma once

#include "Camera.h"
#include "DX.h"

class Menu
{
private:
	Camera* m_camera;
	ID3D11DepthStencilState* m_depthState;

	void initializeResources();

public:
	Menu();
	~Menu();

	ID3D11DepthStencilState* getDepthStencilState();
	Camera* getCamera();
};

