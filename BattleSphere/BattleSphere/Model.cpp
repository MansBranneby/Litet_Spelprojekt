#include "Model.h"

void Model::computeOBB()
{
	const int nrOfPairs = 179;

	// Get 1 degree rotation matrix wa 
	float vx = 0.0f;
	float vy = 1.0f; // Rotate around the Y-axis
	float vz = 0.0f;
	float rotDeg = -0.5f; // 1 degree

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
	float projectedV1, projectedV2;
	for (int i = 0; i < nrOfPairs; i++)
	{
		projectedV1 = (float)XMVector4Dot(firstVertex, pair[i].vec1).m128_f32[0];
		projectedV2 = (float)XMVector4Dot(firstVertex, pair[i].vec2).m128_f32[0];

		pair[i].minLength1 = projectedV1;
		pair[i].maxLength1 = projectedV1;

		pair[i].minLength2 = projectedV2;
		pair[i].maxLength2 = projectedV2;
	}

	// Project each vertex onto each vector, save min and max value 
	// for each vector and their projected difference 
	for (int vectorIndex = 1; vectorIndex < m_nrOfVertices; vectorIndex++)
	{
		XMVECTOR vec = XMVectorSet
		(
			m_vertices[vectorIndex].posX,
			m_vertices[vectorIndex].posY,
			m_vertices[vectorIndex].posZ,
			0.0f
		);

		for (int i = 0; i < nrOfPairs; i++)
		{
			// Project
			projectedV1 = (float)XMVector4Dot(vec, pair[i].vec1).m128_f32[0];
			projectedV2 = (float)XMVector4Dot(vec, pair[i].vec2).m128_f32[0];

			// Compare value and save it if lower or higher
			if (pair[i].maxLength1 < projectedV1)
				pair[i].maxLength1 = projectedV1;
			else if (pair[i].minLength1 > projectedV1)
				pair[i].minLength1 = projectedV1;

			if (pair[i].maxLength2 < projectedV2)
				pair[i].maxLength2 = projectedV2;
			else if (pair[i].minLength2 > projectedV2)
				pair[i].minLength2 = projectedV2;
		}
	}


	// Multiply each pair differences to find the smallest area
	float length1 = pair[0].maxLength1 - pair[0].minLength1;
	float length2 = pair[0].maxLength2 - pair[0].minLength2;
	float smallestArea = length1 * length2;
	int bestPairIndex = 0;
	for (int i = 1; i < nrOfPairs; i++)
	{
		length1 = pair[i].maxLength1 - pair[i].minLength1;
		length2 = pair[i].maxLength2 - pair[i].minLength2;
		float newArea = length1 * length2;

		if (newArea < smallestArea)
		{
			smallestArea = newArea;
			bestPairIndex = i;
		}
	}

	// Calculate and save OBB center
	float vec1Length = 0.5f * (pair[bestPairIndex].maxLength1 + pair[bestPairIndex].minLength1);
	XMVECTOR vec1Center = pair[bestPairIndex].vec1 * vec1Length;

	float vec2Length = 0.5f * (pair[bestPairIndex].maxLength2 + pair[bestPairIndex].minLength2);
	XMVECTOR vec2Center = pair[bestPairIndex].vec2 * vec2Length;

	XMVECTOR bPos = vec1Center + vec2Center;
	m_bData.pos = XMVectorSet(bPos.m128_f32[0], bPos.m128_f32[1], bPos.m128_f32[2], 1.0f);

	// Calculate save half width and depth
	m_bData.halfWD.x = 0.5f * (pair[bestPairIndex].maxLength1 - pair[bestPairIndex].minLength1);
	m_bData.halfWD.y = 0.5f * (pair[bestPairIndex].maxLength2 - pair[bestPairIndex].minLength2);

	// Calculate save the axes
	m_bData.xAxis = pair[bestPairIndex].vec1;
	m_bData.zAxis = pair[bestPairIndex].vec2;
}

void Model::computeMinMaxY()
{
	for (int i = 0; i < m_nrOfVertices; ++i)
	{	
		m_minY = min(m_minY, m_vertices[i].posY);
		m_maxY = max(m_maxY, m_vertices[i].posY);
	}
}

void Model::createVertexBuffer()
{
	m_modelMatrixData = new XMMATRIX();
	*m_modelMatrixData = XMMatrixTranslation(0, 0, 0);

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
	*m_matrixData = m_scalingMat * m_staticRotationMat * m_rotationMat * XMMatrixTranslation(m_pos.m128_f32[0], m_pos.m128_f32[1], m_pos.m128_f32[2]);

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_matrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_matrixData, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(m_matrixCBuffer, 0);
}

void Model::updateRelSubResource()
{
	*m_matrixData = m_scalingMat * m_rotationMat * XMMatrixTranslation(m_pos.m128_f32[0], m_pos.m128_f32[1], m_pos.m128_f32[2])
		* m_relScalingMat * m_relStaticRotationMat * m_relRotationMat * XMMatrixTranslation(m_relPos.m128_f32[0], m_relPos.m128_f32[1], m_relPos.m128_f32[2]);

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

void Model::draw(TextureAnimationData textureAnimationData)
{
	UINT32 vertexSize = sizeof(vertex);
	UINT32 offset = 0;
	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(1, 1, &m_matrixCBuffer);
	textureAnimationData.minY = m_minY;
	textureAnimationData.maxY = m_maxY;
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		m_subModels[i].draw(textureAnimationData);
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
	m_modelMatrixData = nullptr;
	m_staticRotationMat = XMMatrixIdentity();
	m_rotationMat = XMMatrixIdentity();
	m_scalingMat = XMMatrixIdentity();
	m_relStaticRotationMat = XMMatrixIdentity();
	m_relRotationMat = XMMatrixIdentity();
	m_relScalingMat = XMMatrixIdentity();
	m_vertexBuffer = nullptr;
	m_vertexCullingBuffer = nullptr;
	m_vertexAndId = nullptr;

	m_minY = INFINITY;
	m_maxY = -INFINITY;
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

void Model::setStaticRotation(XMVECTOR rotation)
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
	m_staticRotationMat = dRotation;
}

void Model::setStaticRotation(XMVECTOR rotation, XMVECTOR relRotation)
{
	setStaticRotation(rotation);
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
	m_relStaticRotationMat = dRotation;
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

objectData Model::getBVObjectData() const
{
	objectData temp;
	temp.pos = XMVectorSet(XMVectorGetX(m_bData.pos), XMVectorGetY(m_bData.pos), XMVectorGetZ(m_bData.pos), 1.0f);
	float dotProduct = XMVector3Dot(m_bData.xAxis, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)).m128_f32[0];
	float angle = std::acos(dotProduct);
	angle = XMConvertToDegrees(angle);
	temp.rotation = XMVectorSet(0.0f, 1.0f, 0.0f, -angle);
	temp.scale = XMVectorSet(m_bData.halfWD.x, 10.0f, m_bData.halfWD.y, 1.0f);

	return temp;
}

boundingData Model::getBoundingData() const
{
	return m_bData;
}

XMMATRIX Model::getModelMatrix() const
{
	return *m_modelMatrixData;
}

std::vector<XMFLOAT3> Model::getCollisionMesh()
{
	// Make the first element the size of the array.
	float verticeArrSize = float(m_indices.size());
	std::vector<XMFLOAT3> updatedVertices;

	// Get collision mesh updated with latest world matrix
	XMVECTOR temp;
	for (int i = 0; i < verticeArrSize; i++)
	{
		// For each indice create a updated triangle to send
		temp.m128_f32[0] = m_vertices[m_indices[i]].posX;
		temp.m128_f32[1] = m_vertices[m_indices[i]].posY;
		temp.m128_f32[2] = m_vertices[m_indices[i]].posZ;
		temp.m128_f32[3] = 1.0f;

		// Update triangle position with world matrix
		temp = XMVector3Transform(temp, *m_modelMatrixData);

		// Add triangle to list
		updatedVertices.push_back
		(
			XMFLOAT3
			(
				temp.m128_f32[0],
				temp.m128_f32[1],
				temp.m128_f32[2]
			)
		);
	}

	return updatedVertices;
}

std::vector<XMFLOAT3> Model::getCollisionMesh(objectData data)
{
	// Update world matrix
	setObjectData(data);

	// Make the first element the size of the array.
	float verticeArrSize = float(m_indices.size());
	std::vector<XMFLOAT3> updatedVertices;

	// Get collision mesh updated with latest world matrix
	XMVECTOR temp;
	for (int i = 0; i < verticeArrSize; i++)
	{
		// For each indice create a updated triangle to send
		temp.m128_f32[0] = m_vertices[m_indices[i]].posX;
		temp.m128_f32[1] = m_vertices[m_indices[i]].posY;
		temp.m128_f32[2] = m_vertices[m_indices[i]].posZ;
		temp.m128_f32[3] = 1.0f;

		// Update triangle position with world matrix
		temp = XMVector3Transform(temp, *m_modelMatrixData);

		// Add triangle to list
		updatedVertices.push_back
		(
			XMFLOAT3
			(
			temp.m128_f32[0],
			temp.m128_f32[1],
			temp.m128_f32[2]
			)
		);
	}

	return updatedVertices;
}

void Model::setVPMatrix()
{
	*m_matrixData = *m_matrixData * DX::getInstance()->getCam()->getViewMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_matrixCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_matrixData, sizeof(XMMATRIX));
	DX::getInstance()->getDeviceContext()->Unmap(m_matrixCBuffer, 0);
}

std::vector<XMFLOAT3> Model::getCollisionMesh(objectData data, objectData relativeData)
{
	// Update world matrix
	setObjectData(data, relativeData);


	// Make the first element the size of the array.
	float verticeArrSize = float(m_indices.size());
	std::vector<XMFLOAT3> updatedVertices;

	// Get collision mesh updated with latest world matrix
	XMVECTOR temp;
	for (int i = 0; i < verticeArrSize; i++)
	{
		// For each indice create a updated triangle to send
		temp.m128_f32[0] = m_vertices[m_indices[i]].posX;
		temp.m128_f32[1] = m_vertices[m_indices[i]].posY;
		temp.m128_f32[2] = m_vertices[m_indices[i]].posZ;
		temp.m128_f32[3] = 1.0f;

		// Update triangle position with world matrix
		temp = XMVector3Transform(temp, *m_modelMatrixData);

		// Add triangle to list
		updatedVertices.push_back
		(
			XMFLOAT3
			(
				temp.m128_f32[0],
				temp.m128_f32[1],
				temp.m128_f32[2]
			)
		);
	}

	return updatedVertices;
}

void Model::setObjectData(objectData data, int modelNr)
{
	setPosition(data.pos);
	setStaticRotation(data.staticRotation);
	setRotation(data.rotation);
	setScale(data.scale);
	if (modelNr != -1)
		m_subModels[modelNr].updateMaterialInfo(data.material);
	updateSubResource();
}

void Model::setObjectData(objectData data, objectData relativeData, int modelNr)
{
	setPosition(data.pos, relativeData.pos);
	setStaticRotation(data.staticRotation, relativeData.staticRotation);
	setRotation(data.rotation, relativeData.rotation);
	setScale(data.scale, relativeData.scale);
	if (modelNr != -1)
		m_subModels[modelNr].updateMaterialInfo(data.material);
	updateRelSubResource();
}

void Model::setAllObjectData(objectData data)
{
	setPosition(data.pos);
	setStaticRotation(data.staticRotation);
	setRotation(data.rotation);
	setScale(data.scale);
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		m_subModels[i].updateMaterialInfo(data.material);
	}
	updateSubResource();
}

void Model::setAllObjectData(objectData data, objectData relativeData)
{
	setPosition(data.pos, relativeData.pos);
	setStaticRotation(data.staticRotation, relativeData.staticRotation);
	setRotation(data.rotation, relativeData.rotation);
	setScale(data.scale, relativeData.scale);
	for (int i = 0; i < m_nrOfSubModels; i++)
	{
		m_subModels[i].updateMaterialInfo(data.material);
	}
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
		std::string hey;
		std::getline(in, line);
		inputStream.str(line);
		inputStream >> hey;
		inputStream.clear();

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

			// Save vertex indices 
			m_indices.push_back(indices[j]);
			m_indices.push_back(indices[j + 1]);
			m_indices.push_back(indices[j + 2]);
			inputStream.clear();
		}
		m_subModels[i].setFaces(indices, nrOfIndices);
	}

	// Create model matrix
	createVertexCBuffer();

	// Create bounding volume
	computeOBB(); // Calculate information for bounding volume data
}

void Model::loadModel(std::ifstream& in, ObjectType type)
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
	ID3D11ShaderResourceView* tempSRV;
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

		//// Diffuse texture
		std::getline(in, line);
		if (!line.empty() && *line.rbegin() == '\r') 
			line.erase(line.length() - 1, 1);
		inputStream.str(line);
		tempSRV = createTexture(inputStream.str()); // Create texture
		inputStream.clear();

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
		// Set submodel texture
		m_subModels[i].setSRV(tempSRV);

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
			int index0 = j;
			int index1 = j + 1;
			int index2 = j + 2;

			std::getline(in, line);
			inputStream.str(line);
			inputStream >> indices[index0] >> indices[index1] >> indices[index2];

			// Save vertex indices 
			m_indices.push_back(indices[index0]);
			m_indices.push_back(indices[index1]);
			m_indices.push_back(indices[index2]);
			inputStream.clear();
		}
		m_subModels[i].setFaces(indices, nrOfIndices);
	}

	// Create model matrix
	createVertexCBuffer();

	// Create bounding volume
	computeOBB(); // Calculate information for bounding volume data
	computeMinMaxY();
}

ID3D11ShaderResourceView* Model::createTexture(std::string fileName)
{
	ID3D11ShaderResourceView* tempSRV = nullptr;

	if (fileName != "\r" && fileName != "")
	{
		std::wstring wFileName(fileName.length(), L' ');
		std::copy(fileName.begin(), fileName.end(), wFileName.begin());

		HRESULT hr = CoInitialize(NULL);
		hr = CreateWICTextureFromFile(DX::getInstance()->getDevice(), wFileName.c_str(), NULL, &tempSRV);

		if (FAILED(hr))
			MessageBox(NULL, L"Error createTexture in Model.cpp", L"Error", MB_OK | MB_ICONERROR);
	}

	return tempSRV;
}
