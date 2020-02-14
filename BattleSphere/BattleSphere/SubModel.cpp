#include "SubModel.h"

SubModel::SubModel()
{
	m_indexBuffer = nullptr;
	m_materialCBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_mat = (material*)_aligned_malloc(sizeof(material), 16);
	m_indexArray = nullptr;

}

SubModel::~SubModel()
{
	if (m_indexBuffer) m_indexBuffer->Release();

	if (m_constantBuffer) delete m_constantBuffer;
	if(m_mat) _aligned_free(m_mat);
	if(m_indexArray) delete[] m_indexArray;
	

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
}

void SubModel::draw()
{
	//Bind indexbuffer
	DX::getInstance()->getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Bind constantbuffer TODO: Change startslot
	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, &this->m_materialCBuffer);
	DX::getInstance()->getDeviceContext()->DrawIndexed(m_nrOfIndices, 0, 0);
	
}
