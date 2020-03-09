#include "LineShots.h"

LineShots::LineShots()
{
	for (int i = 0; i < MAX_USER; i++)
	{
		m_lines[i][0] = XMVectorSet(-10.0f, 2.5f, 0.0f, 0.0f);
		m_lines[i][1] = XMVectorSet(10.0f, 2.5f, 0.0f, 0.0f);
		m_anim[i] = 0.0f;
		m_widthAlpha[i].x = 0.0f;
		m_widthAlpha[i].y = 0.0f;
		m_active[i] = false;
		m_animOn[i] = false;
		m_colour[i] = XMVectorSet(1,1,1,0);
	}

	m_animCBuffer = new ConstantBuffer(&m_widthAlpha[0], sizeof(XMFLOAT4));
	m_colourCBuffer = new ConstantBuffer(&m_colour[0], sizeof(XMVECTOR));

	m_vs = VertexShader(L"VertexShaderPath.hlsl", 2);
	m_gs = GeometryShader(L"GeometryShaderLineShots.hlsl");
	m_ps = PixelShader(L"PixelShaderLineShots.hlsl");
}

LineShots::~LineShots()
{
	m_vs.release();
	m_gs.release();
	m_ps.release();
	m_vsBuffer->Release();
	delete m_animCBuffer;
	delete m_colourCBuffer;
}

void LineShots::createVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(XMVECTOR) * 2;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	// create a Vertex Buffer
	HRESULT result = DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, nullptr, &m_vsBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"LineShots VB", L"Error", MB_OK | MB_ICONERROR);
}

void LineShots::setColour(int index, XMVECTOR colour)
{
	m_colour[index] = colour;
}

void LineShots::setActive(int index, bool active)
{
	m_active[index] = active;
}

void LineShots::updateLineStatus(int index, XMVECTOR start, XMVECTOR end, bool active, float dt)
{
	m_lines[index][0] = start;
	m_lines[index][1] = end;
	if (active && !m_animOn[index])
	{
		m_animOn[index] = true;
	}

	if (m_animOn[index])
	{
		m_anim[index] += dt;
	}

	m_widthAlpha[index].x = max(0.15f, 0.15f + sin(m_anim[index] * 15));//(sin(pow(m_anim[index], 5.0f) / 2.0f) / 8.0f);
	m_widthAlpha[index].y = max(0.3f, 0.3f + sin(m_anim[index] * 15) / 2.5f);//(sin(pow(m_anim[index], 5.0f) / 2.0f) / 1.5f);
	if (m_anim[index] > 0.3f)
	{
		m_animOn[index] = false;
		m_anim[index] = 0.0f;
	}
}

void LineShots::draw(int index)
{
	if (m_active[index])
	{
		D3D11_MAPPED_SUBRESOURCE mappedMemory;

		DX::getInstance()->getDeviceContext()->Map(m_vsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_lines[index][0], sizeof(XMVECTOR) * 2);
		DX::getInstance()->getDeviceContext()->Unmap(m_vsBuffer, 0);

		DX::getInstance()->getDeviceContext()->Map(*m_animCBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_widthAlpha[index], sizeof(XMFLOAT4));
		DX::getInstance()->getDeviceContext()->Unmap(*m_animCBuffer->getConstantBuffer(), 0);

		DX::getInstance()->getDeviceContext()->Map(*m_colourCBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_colour[index], sizeof(XMVECTOR));
		DX::getInstance()->getDeviceContext()->Unmap(*m_colourCBuffer->getConstantBuffer(), 0);

		DX::getInstance()->getDeviceContext()->VSSetShader(&m_vs.getVertexShader(), nullptr, 0);
		DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
		DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
		DX::getInstance()->getDeviceContext()->GSSetShader(&m_gs.getGeometryShader(), nullptr, 0);
		DX::getInstance()->getDeviceContext()->PSSetShader(&m_ps.getPixelShader(), nullptr, 0);
		DX::getInstance()->getDeviceContext()->GSSetConstantBuffers(1, 1, m_animCBuffer->getConstantBuffer());
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_colourCBuffer->getConstantBuffer());
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, m_animCBuffer->getConstantBuffer());
		UINT32 vertexSize = sizeof(XMVECTOR);
		UINT32 offset = 0;

		DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vsBuffer, &vertexSize, &offset);
		DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		DX::getInstance()->getDeviceContext()->IASetInputLayout(&m_vs.getvertexLayout());
		DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);

		if(m_animOn[index]) // If shooting, enable emission
			DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSEnabled(), 1);
		DX::getInstance()->getDeviceContext()->Draw(2, 0);

		// Disable emission
		DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);
	}
}
