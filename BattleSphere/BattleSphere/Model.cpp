#include "Model.h"


void Model::createVertexBuffer()
{
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

void Model::createVertexCullingBuffer()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertexAndId) * m_nrOfVertices;
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = m_vertexAndId;
	DX::getInstance()->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexCullingBuffer);
}

void Model::createVertexCBuffer()
{
	m_matrixData = new XMMATRIX();
	*m_matrixData = XMMatrixTranslation(0, 0, 0);

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
	vsCBufferData.pSysMem = m_matrixData;
	vsCBufferData.SysMemPitch = 0;
	vsCBufferData.SysMemSlicePitch = 0;
	DX::getInstance()->getDevice()->CreateBuffer(&vsCBufferDesc, &vsCBufferData, &m_matrixCBuffer);
}

void Model::updateSubResource()
{
	*m_matrixData = m_scalingMat * m_rotationMat * XMMatrixTranslation(m_pos.m128_f32[0], m_pos.m128_f32[1], m_pos.m128_f32[2]);

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_matrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_matrixData, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(m_matrixCBuffer, 0);
}

void Model::updateRelSubResource()
{
	*m_matrixData = m_scalingMat * m_rotationMat * XMMatrixTranslation(m_pos.m128_f32[0], m_pos.m128_f32[1], m_pos.m128_f32[2])
		* m_relScalingMat * m_relRotationMat * XMMatrixTranslation(m_relPos.m128_f32[0], m_relPos.m128_f32[1], m_relPos.m128_f32[2]);

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_matrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_matrixData, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(m_matrixCBuffer, 0);
}

void Model::draw()
{
	UINT32 vertexSize = sizeof(vertex);
	UINT32 offset = 0;
	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(1, 1, &m_matrixCBuffer);
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		m_subModels[i].draw();
	}
}

void Model::cullDraw()
{
	UINT32 vertexSize = sizeof(vertexAndId);
	UINT32 offset = 0;
	setVPMatrix(); // Set world view matrix instead of only world matrix
	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexCullingBuffer, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(1, 1, &m_matrixCBuffer);
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		m_subModels[i].cullDraw();
	}
	updateSubResource(); // Reset world matrix
}

Model::Model()
{
	m_pos = XMVectorSet(0, 0, 0, 0);
	m_relPos = XMVectorSet(0, 0, 0, 0);
	m_vertices = nullptr;
	m_subModels = nullptr;
	m_vertexBuffer = nullptr;
	m_nrOfSubModels = 0;
	m_nrOfVertices = 0;
	m_matrixCBuffer = nullptr;
	m_matrixData = nullptr;
	m_modelMatrix = XMMatrixIdentity();
	m_rotationMat = XMMatrixIdentity();
	m_rotationAfterMat = XMMatrixIdentity();
	m_scalingMat = XMMatrixIdentity();
	m_relRotationMat = XMMatrixIdentity();
	m_relScalingMat = XMMatrixIdentity();
	m_vertexBuffer = nullptr;
	m_vertexCullingBuffer = nullptr;
	m_vertexAndId = nullptr;
}

Model::~Model()
{
	if (m_vertices) delete[] m_vertices;
	if (m_subModels) delete[] m_subModels;
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_matrixCBuffer) m_matrixCBuffer->Release();
	if (m_matrixData) delete (m_matrixData);
	if (m_vertexCullingBuffer) m_vertexCullingBuffer->Release();
	if (m_vertexAndId) delete[] m_vertexAndId;
}

void Model::setPosition(XMVECTOR pos)
{
	m_pos = pos;
}

void Model::setPosition(XMVECTOR pos, XMVECTOR relPos)
{
	m_pos = pos;
	m_relPos = relPos;
}

void Model::setRotation(XMVECTOR rotation)
{
	float vx = rotation.m128_f32[0];
	float vy = rotation.m128_f32[1];
	float vz = rotation.m128_f32[2];
	float rotDeg = rotation.m128_f32[3];

	float rotInRad = XMConvertToRadians(rotDeg);
	vx *= sin(rotInRad / 2);
	vy *= sin(rotInRad / 2);
	vz *= sin(rotInRad / 2);
	float rot = cos(rotInRad / 2);
	XMVECTOR rotVec = { vx, vy, vz,  rot };
	XMMATRIX dRotation = XMMatrixRotationQuaternion(rotVec);
	m_rotationMat = dRotation;
}

void Model::setRotation(XMVECTOR rotation, XMVECTOR relRotation)
{
	setRotation(rotation);
	float vx = relRotation.m128_f32[0];
	float vy = relRotation.m128_f32[1];
	float vz = relRotation.m128_f32[2];
	float rotDeg = relRotation.m128_f32[3];

	float rotInRad = XMConvertToRadians(rotDeg);
	vx *= sin(rotInRad / 2);
	vy *= sin(rotInRad / 2);
	vz *= sin(rotInRad / 2);
	float rot = cos(rotInRad / 2);
	XMVECTOR rotVec = { vx, vy, vz,  rot };
	XMMATRIX dRotation = XMMatrixRotationQuaternion(rotVec);
	m_relRotationMat = dRotation;
}

void Model::setScale(XMVECTOR scale)
{
	float xScale = scale.m128_f32[0];
	float yScale = scale.m128_f32[1];
	float zScale = scale.m128_f32[2];
	m_scalingMat = XMMatrixScaling(xScale, yScale, zScale);
}

void Model::setScale(XMVECTOR scale, XMVECTOR relScale)
{
	setScale(scale);
	float xScale = relScale.m128_f32[0];
	float yScale = relScale.m128_f32[1];
	float zScale = relScale.m128_f32[2];
	m_relScalingMat = XMMatrixScaling(xScale, yScale, zScale);
}

void Model::setVPMatrix()
{
	*m_matrixData = *m_matrixData * DX::getInstance()->getCam()->getViewMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_matrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_matrixData, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(m_matrixCBuffer, 0);
}


void Model::setObjectData(objectData data, int modelNr)
{
	setPosition(data.pos);
	setRotation(data.rotation);
	setScale(data.scale);
	if (modelNr != -1)
	m_subModels[modelNr].updateMaterialInfo(data.material);
	updateSubResource();
}

void Model::setObjectData(objectData data, objectData relativeData, int modelNr)
{
	setPosition(data.pos, relativeData.pos);
	setRotation(data.rotation, relativeData.rotation);
	setScale(data.scale, relativeData.scale);
	if (modelNr != -1)
		m_subModels[modelNr].updateMaterialInfo(data.material);
	updateRelSubResource();
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
	m_vertexAndId = new vertexAndId[m_nrOfVertices];

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
		m_vertexAndId[i].posX = m_vertices[i].posX;
		m_vertexAndId[i].posY = m_vertices[i].posY;
		m_vertexAndId[i].posZ = m_vertices[i].posZ;
		m_vertexAndId[i].iD = i;
	}
	// Creates a vertex buffer
	createVertexBuffer();

	// Creates the vertex culling buffer, contains position and vertex ID
	createVertexCullingBuffer();


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
