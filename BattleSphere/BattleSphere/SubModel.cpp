#include "SubModel.h"

SubModel::SubModel()
{
	// Model data
	m_indexBuffer = nullptr;
	m_materialCBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_mat = (material*)_aligned_malloc(sizeof(material), 16);
	m_nrOfIndices = 0;
	m_indexArray = nullptr;

	// Backface culling
	m_culledIndiceBuffer = nullptr;
	m_clearedIndiceBuffer = nullptr;
	m_nrOfCulledIndices = 0;
}

SubModel::~SubModel()
{
	if (m_indexBuffer) m_indexBuffer->Release();

	if (m_constantBuffer) delete m_constantBuffer;
	if (m_mat) _aligned_free(m_mat);
	if (m_indexArray) delete[] m_indexArray;
	if (m_culledIndiceBuffer) m_culledIndiceBuffer->Release();
	if (m_clearedIndiceBuffer) m_clearedIndiceBuffer->Release();
}

void SubModel::createIndexBuffer()
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * m_nrOfIndices;
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = m_indexArray;
	DX::getInstance()->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
}

void SubModel::createCulledIndexBuffer()
{
	D3D11_BUFFER_DESC culledIndexBufferDesc;
	ZeroMemory(&culledIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	culledIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	culledIndexBufferDesc.StructureByteStride = sizeof(UINT);
	culledIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	culledIndexBufferDesc.ByteWidth = sizeof(int) * m_nrOfIndices;

	DX::getInstance()->getDevice()->CreateBuffer(&culledIndexBufferDesc, nullptr, &m_clearedIndiceBuffer);

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = m_indexArray;

	DX::getInstance()->getDevice()->CreateBuffer(&culledIndexBufferDesc, &indexData, &m_culledIndiceBuffer);
}

void SubModel::setMaterialInfo(material mat)
{
	*m_mat = mat;

	//TODO: Create constant buffer
	m_constantBuffer = new ConstantBuffer(m_mat, sizeof(material));
	m_materialCBuffer = *m_constantBuffer->getConstantBuffer();
}

void SubModel::updateMaterialInfo(material mat)
{
	for (int i = 0; i < 4; i++)
	{
		m_mat->ambient.m128_f32[i] = (mat.ambient.m128_f32[i] != -1) ? mat.ambient.m128_f32[i] : m_mat->ambient.m128_f32[i];
		m_mat->diffuse.m128_f32[i] = (mat.diffuse.m128_f32[i] != -1) ? mat.diffuse.m128_f32[i] : m_mat->diffuse.m128_f32[i];
		m_mat->specular.m128_f32[i] = (mat.specular.m128_f32[i] != -1) ? mat.specular.m128_f32[i] : m_mat->specular.m128_f32[i];
		m_mat->emission.m128_f32[i] = (mat.emission.m128_f32[i] != -1) ? mat.emission.m128_f32[i] : m_mat->emission.m128_f32[i];
	}

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_materialCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_mat, sizeof(material));
	DX::getInstance()->getDeviceContext()->Unmap(m_materialCBuffer, 0);
}

void SubModel::setFaces(int* indexBuffer, int nrOfIndices)
{
	m_indexArray = indexBuffer;
	m_nrOfIndices = nrOfIndices;

	createIndexBuffer();
	createCulledIndexBuffer();
}

void SubModel::draw()
{
	// Bind indexbuffer
	DX::getInstance()->getDeviceContext()->IASetIndexBuffer(m_culledIndiceBuffer, DXGI_FORMAT_R32_UINT, 0);

	DX* instance = DX::getInstance();
	XMVECTOR emission = m_mat->emission;
	if (emission.m128_f32[0] > 0 || emission.m128_f32[1] > 0 || emission.m128_f32[2] > 0)
		instance->getDeviceContext()->OMSetDepthStencilState(instance->getDSSEnabled(), 1);
	else
		instance->getDeviceContext()->OMSetDepthStencilState(instance->getDSSDisabled(), 1);

	// Bind constantbuffer
	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, &this->m_materialCBuffer);
	DX::getInstance()->getDeviceContext()->DrawIndexed(m_nrOfIndices, 0, 0);
	
}

void SubModel::cullDraw()
{
	// Bind indexbuffer
	DX::getInstance()->getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Clear culled index buffer
	DX::getInstance()->getDeviceContext()->CopyResource(m_culledIndiceBuffer, m_clearedIndiceBuffer);

	// Bind geometry stream output shader
	DX::getInstance()->getDeviceContext()->SOSetTargets(1, &m_culledIndiceBuffer, NULL);

	// Draw
	DX::getInstance()->getDeviceContext()->DrawIndexed(m_nrOfIndices, 0, 0);

	// Unbind index buffer as output shader
	DX::getInstance()->getDeviceContext()->SOSetTargets(NULL, nullptr, NULL);
}
