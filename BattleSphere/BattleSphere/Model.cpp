#include "Model.h"


void Model::createVertexBuffer()
{
	//TODO: Remove
	/*for (int i = 0; i < m_nrOfVertices; i++)
	{
		OutputDebugStringA((std::to_string(m_vertices[i].normX) + " " + std::to_string(m_vertices[i].normY) + " " + std::to_string(m_vertices[i].normZ) + '\n').c_str());
	}*/
	
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertex) * m_nrOfVertices;
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = m_vertices;
	DX::getInstance()->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
}

void Model::createVertexCBuffer()
{
	m_modelMatrixData = new XMMATRIX();
	*m_modelMatrixData = XMMatrixTranslation(0, 0, 0);

	D3D11_BUFFER_DESC vsCBufferDesc;
	ZeroMemory(&vsCBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vsCBufferDesc.ByteWidth = sizeof(XMMATRIX);
	vsCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vsCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vsCBufferDesc.MiscFlags = 0;
	vsCBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vsCBufferData;
	ZeroMemory(&vsCBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	vsCBufferData.pSysMem = m_modelMatrixData;
	vsCBufferData.SysMemPitch = 0;
	vsCBufferData.SysMemSlicePitch = 0;

	DX::getInstance()->getDevice()->CreateBuffer(&vsCBufferDesc, &vsCBufferData, &m_modelMatrixCBuffer);
}

void Model::updateSubResource()
{
	*m_modelMatrixData = m_scalingMat * m_rotationMat * XMMatrixTranslation(m_pos.m128_f32[0], m_pos.m128_f32[1], m_pos.m128_f32[2]);

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_modelMatrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_modelMatrixData, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(m_modelMatrixCBuffer, 0);
}

void Model::draw()
{
	UINT32 vertexSize = sizeof(vertex);
	UINT32 offset = 0;
	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(1, 1, &m_modelMatrixCBuffer);
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		m_subModels[i].draw();
	}
}

Model::Model()
{
	m_pos = XMVectorSet(0, 0, 0, 0);
	m_vertices = nullptr;
	m_subModels = nullptr;
	m_vertexBuffer = nullptr;
	m_nrOfSubModels = 0;
	m_nrOfVertices = 0;
	m_modelMatrixCBuffer = nullptr;
	m_modelMatrixData = nullptr;
	m_modelMatrix = XMMatrixIdentity();
	m_rotationMat = XMMatrixIdentity();
	m_scalingMat = XMMatrixIdentity();
}

Model::~Model()
{
	if (m_vertices) delete[] m_vertices;
	if (m_subModels) delete[] m_subModels;
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_modelMatrixCBuffer) m_modelMatrixCBuffer->Release();
	if (m_modelMatrixData) delete (m_modelMatrixData);
}

void Model::move(XMVECTOR dPos)
{
	m_pos += dPos;
}

void Model::rotate(float vx, float vy, float vz, float rotDeg)
{
	float rotInRad = XMConvertToRadians(rotDeg);
	vx *= sin(rotInRad / 2);
	vy *= sin(rotInRad / 2);
	vz *= sin(rotInRad / 2);
	float rotation = cos(rotInRad / 2);
	XMVECTOR rotVec = { vx, vy, vz,  rotation };
	XMMATRIX dRotation = XMMatrixRotationQuaternion(rotVec);
	m_rotationMat = m_rotationMat * dRotation;
	updateSubResource();
}

void Model::scale(float xScale, float yScale, float zScale)
{
	m_scalingMat = XMMatrixScaling(xScale, yScale, zScale);
	updateSubResource();
}

void Model::setPosition(XMVECTOR pos)
{
	m_pos = pos;
	updateSubResource();
}

void Model::loadModel(std::ifstream& in)
{
	std::string line;
	std::istringstream inputStream;

	std::getline(in, line);
	inputStream.str(line);
	inputStream >> m_nrOfVertices;
	inputStream.clear();
	m_vertices = new vertex[m_nrOfVertices];

	// Read vertices
	for (int i = 0; i < m_nrOfVertices; i++)
	{
		std::getline(in, line);
		inputStream.str(line); 
		inputStream >>
			m_vertices[i].posX >> m_vertices[i].posY >> m_vertices[i].posZ >>
			m_vertices[i].u >> m_vertices[i].v >>
			m_vertices[i].normX >> m_vertices[i].normY >> m_vertices[i].normZ;
		inputStream.clear();
	}
	// Creates a vertex buffer
	createVertexBuffer();


	// Get number of materials to read
	std::getline(in, line);
	inputStream.str(line);
	inputStream >> m_nrOfSubModels;
	inputStream.clear();

	m_subModels = new SubModel[m_nrOfSubModels];
	material tempMat;
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		// Ambient
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.ambient.m128_f32[0] >> tempMat.ambient.m128_f32[1] >> tempMat.ambient.m128_f32[2];
		inputStream.clear();

		// Diffuse
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.diffuse.m128_f32[0] >> tempMat.diffuse.m128_f32[1] >> tempMat.diffuse.m128_f32[2];
		inputStream.clear();

		// Specular
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.specular.m128_f32[0] >> tempMat.specular.m128_f32[1] >> tempMat.specular.m128_f32[2];
		inputStream.clear();

		// Emission
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.emission.m128_f32[0] >> tempMat.emission.m128_f32[1] >> tempMat.emission.m128_f32[2];
		inputStream.clear();

		// Shininess
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.specular.m128_f32[3];
		inputStream.clear();

		// Diffuse texture
		
		std::getline(in, line);

		//?

		// Refraction
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.diffuse.m128_f32[3];
		inputStream.clear();


		// Opacity
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.emission.m128_f32[3];
		inputStream.clear();

		// Illumination model enumeration
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> tempMat.ambient.m128_f32[3];
		inputStream.clear();

		// Set submodel material
		m_subModels[i].setMaterialInfo(tempMat);

		// Get number of indices
		int nrOfIndices = 0;
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> nrOfIndices;
		inputStream.clear();

		// Read indices
		int* indices = new int[nrOfIndices];
		for (int j = 0; j < nrOfIndices; j+=3)
		{
			std::getline(in, line);
			inputStream.str(line);
			inputStream >> indices[j] >> indices[j+1] >> indices[j+2];
			inputStream.clear();
		}
		m_subModels[i].setFaces(indices, nrOfIndices);
	}

	// Create model matrix
	createVertexCBuffer();
}
