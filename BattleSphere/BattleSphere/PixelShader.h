#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DX.h"

class PixelShader
{
private:
	ID3D11PixelShader* m_pixelShader;
	HRESULT createPixelShader(LPCWSTR fileName);
public:
	PixelShader();
	PixelShader(LPCWSTR fileName);

	ID3D11PixelShader& getPixelShader() const;
};