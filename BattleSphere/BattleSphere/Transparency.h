#pragma once
#include "ConstantBuffer.h"
#include <DirectXMath.h>
using namespace DirectX;
struct robotInfo {
	XMFLOAT4 position[4];

};
class Transparency
{
	
private:
	
	ConstantBuffer* m_constantBuffer;
	robotInfo* m_info;
public:
	void initialize();
	void update(XMVECTOR position, XMMATRIX viewMatrix, XMMATRIX projMatrix, int index);
	void bindConstantBuffer();
	Transparency();
	~Transparency();
};

