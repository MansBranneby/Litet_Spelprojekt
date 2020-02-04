#include "Model.h"


void Model::computeOBB()
{
	const int nrOfPairs = 89;

	// Get 1 degree rotation matrix
	float vx = 0.0f;
	float vy = 1.0f; // Rotate around the Y-axis
	float vz = 0.0f;
	float rotDeg = -1.0f; // 1 degree

	float rotInRad = XMConvertToRadians(rotDeg);
	vx *= sin(rotInRad / 2);
	vy *= sin(rotInRad / 2);
	vz *= sin(rotInRad / 2);
	float rot = cos(rotInRad / 2);
	XMVECTOR rotVec = { vx, vy, vz,  rot };
	XMMATRIX rotMat = XMMatrixRotationQuaternion(rotVec);


	// Create 89 orthonormal vector pairs with 1 degree between them
	vectorPairProjections pair[nrOfPairs];


	for (int i = 1; i < nrOfPairs; i++)
	{
		// Rotate the previous vector one degree and save it to the next one
		pair[i].vec1 = XMVector3Transform(pair[i - 1].vec1, rotMat);
		pair[i].vec2 = XMVector3Transform(pair[i - 1].vec2, rotMat);
	}

	// Set all min max values to the first vertex
	XMVECTOR firstVertex = XMVectorSet(m_vertices[0].posX, m_vertices[0].posY, m_vertices[0].posZ, 0.0f);
	for (int i = 0; i < nrOfPairs; i++)
	{
		pair[i].minVec1 = firstVertex;
		pair[i].maxVec1 = firstVertex;
		pair[i].minVec2 = firstVertex;
		pair[i].maxVec2 = firstVertex;
	}

	// Project each vertex onto each vector, save min and max value 
	// for each vector and their projected difference 


	// Multiply each pair differences to find the smallest area


	// Calculate and save OBB center, half width and depth, as well as the axes
	
}

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

void Model::updateRelSubResource()
{
	*m_modelMatrixData = m_scalingMat * m_rotationMat * XMMatrixTranslation(m_pos.m128_f32[0], m_pos.m128_f32[1], m_pos.m128_f32[2])
		* m_relScalingMat * m_relRotationMat * XMMatrixTranslation(m_relPos.m128_f32[0], m_relPos.m128_f32[1], m_relPos.m128_f32[2]);

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
	m_relPos = XMVectorSet(0, 0, 0, 0);
	m_vertices = nullptr;
	m_subModels = nullptr;
	m_vertexBuffer = nullptr;
	m_nrOfSubModels = 0;
	m_nrOfVertices = 0;
	m_modelMatrixCBuffer = nullptr;
	m_modelMatrixData = nullptr;
	m_modelMatrix = XMMatrixIdentity();
	m_rotationMat = XMMatrixIdentity();
	m_rotationAfterMat = XMMatrixIdentity();
	m_scalingMat = XMMatrixIdentity();
	m_relRotationMat = XMMatrixIdentity();
	m_relScalingMat = XMMatrixIdentity();
}

Model::~Model()
{
	if (m_vertices) delete[] m_vertices;
	if (m_subModels) delete[] m_subModels;
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_modelMatrixCBuffer) m_modelMatrixCBuffer->Release();
	if (m_modelMatrixData) delete (m_modelMatrixData);
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


void Model::setObjectData(objectData data)
{
	setPosition(data.pos);
	setRotation(data.rotation);
	setScale(data.scale);
	updateSubResource();
}

void Model::setObjectData(objectData data, objectData relativeData)
{
	setPosition(data.pos, relativeData.pos);
	setRotation(data.rotation, relativeData.rotation);
	setScale(data.scale, relativeData.scale);
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
		for (int j = 0; j < nrOfIndices; j += 3)
		{
			std::getline(in, line);
			inputStream.str(line);
			inputStream >> indices[j] >> indices[j + 1] >> indices[j + 2];
			inputStream.clear();
		}
		m_subModels[i].setFaces(indices, nrOfIndices);
	}

	// Create model matrix
	createVertexCBuffer();

	// Create bounding volume
	computeOBB();
}
