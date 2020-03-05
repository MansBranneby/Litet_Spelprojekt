#pragma once

#include "StructsAndEnums.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "VertexShader.h"

#define MAX_USER 4

class LineShots
{
private:
	XMVECTOR m_lines[MAX_USER][2];
	XMFLOAT4 m_widthAlpha[MAX_USER];
	float m_anim[MAX_USER];
	bool m_animOn[MAX_USER];
	bool m_active[MAX_USER];
	XMVECTOR m_colour[MAX_USER];

	VertexShader m_vs;
	GeometryShader m_gs;
	PixelShader m_ps;
	ConstantBuffer* m_animCBuffer;
	ConstantBuffer* m_colourCBuffer;
	ID3D11Buffer* m_vsBuffer = nullptr;

public:
	LineShots();
	~LineShots();

	void createVertexBuffer();
	void setColour(int index, XMVECTOR colour);
	void setActive(int index, bool active);

	void updateLineStatus(int index, XMVECTOR start, XMVECTOR end, bool active, float dt);
	void draw(int index);
};

