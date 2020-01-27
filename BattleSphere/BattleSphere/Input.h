#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Xinput.h>
#include "DX.h"

using namespace DirectX;

class Input
{
private:
	
public:
	Input();

	int getControllerState();
};