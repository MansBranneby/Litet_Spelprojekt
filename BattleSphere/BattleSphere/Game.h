#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"

using namespace DirectX;

class Game
{
private:
	std::vector<GameObject*> m_gameObjects;
public:
	Game(float dt);

	void update();
	void draw();
};