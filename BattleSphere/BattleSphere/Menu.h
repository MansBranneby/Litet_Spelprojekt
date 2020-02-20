#pragma once

#include "DX.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"

class Menu
{
private:
	Camera* m_camera;
	Camera* m_cameraPerspective;
//	ID3D11DepthStencilState* m_depthState;
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;

	void initializeResources();

public:
	Menu();
	~Menu();

	//ID3D11DepthStencilState* getDepthStencilState();
	Camera* getCamera(bool perspective);
	VertexShader* getVertexShader();
	PixelShader* getPixelShader();
};

