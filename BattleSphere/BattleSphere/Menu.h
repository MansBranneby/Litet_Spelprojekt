#pragma once

#include "DX.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "WICTextureLoader.h"
#include <string>
#include <wchar.h>

class Menu
{
private:
	Camera* m_camera;
	Camera* m_cameraPerspective;
//	ID3D11DepthStencilState* m_depthState;
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	PixelShader* m_pixelShaderAni;
	PixelShader* m_pixelShaderUI;
	PixelShader* m_pixelShaderLoadingScreen;

	ID3D11ShaderResourceView* m_loadingScreenSRV;

	void initializeResources();

public:
	Menu();
	~Menu();

	//ID3D11DepthStencilState* getDepthStencilState();
	Camera* getCamera(bool perspective);
	VertexShader* getVertexShader();
	PixelShader* getPixelShader(int index);

	void setLoadingScreenSRV();
};

