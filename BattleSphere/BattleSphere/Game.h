#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
#include "Input.h"

using namespace DirectX;

class Game
{
private:
	std::vector<GameObject*> m_gameObjects;
	Input input;
public:
	Game();

	void update(float dt);
	void draw();

	void release();
};