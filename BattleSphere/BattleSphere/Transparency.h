#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
using namespace DirectX;
struct robotInfo {
	XMFLOAT3 position;
	float radius;
};
class Transparency
{
	
private:
	
	ConstantBuffer* m_constantBuffer;
	robotInfo* m_info;
public:
	void initialize();
	void update(XMVECTOR position, XMMATRIX viewMatrix, XMMATRIX projMatrix);
	void bindConstantBuffer();
	Transparency();
	~Transparency();
};

