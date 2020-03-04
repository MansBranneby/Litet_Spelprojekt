#include "Particles.h"

float Particles::randF()
{
	return 2.0f * ((float)rand() - (float)RAND_MAX / 2.0f) / (float)RAND_MAX;
}

Particles::Particles()
{
	// Create shaders
	m_computeShaderParticles = ComputeShader(L"ComputeShaderParticles.hlsl");
	m_vertexShaderParticles = VertexShader(L"VertexShaderParticles.hlsl", 2);
	m_geometryShaderParticles = GeometryShader(L"GeometryShaderParticles.hlsl");
	m_pixelShaderParticles = PixelShader(L"PixelShaderParticles.hlsl");

	// Set prev and updated pointers to definite particle buffers
	m_prevParticles = &m_actualBuffers[0];
	m_updatedParticles = &m_actualBuffers[1];

	// Create compute shader particle structured buffers and unordered access views
	D3D11_BUFFER_DESC sBDesc = {};
	sBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	sBDesc.Usage = D3D11_USAGE_DEFAULT;
	sBDesc.CPUAccessFlags = 0;
	sBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sBDesc.StructureByteStride = sizeof(particle);
	sBDesc.ByteWidth = sizeof(particle) * MAX_PARTICLES;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Buffer.NumElements = MAX_PARTICLES;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Buffer.NumElements = MAX_PARTICLES;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sBDesc, 0, &m_prevParticles->sBParticles)))
	{
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_prevParticles->sBParticles, &uavDesc, &m_prevParticles->sBParticlesUAV);
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_prevParticles->sBParticles, &srvDesc, &m_prevParticles->sBParticlesSRV);
	}

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&sBDesc, 0, &m_updatedParticles->sBParticles)))
	{
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_updatedParticles->sBParticles, &uavDesc, &m_updatedParticles->sBParticlesUAV);
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_updatedParticles->sBParticles, &srvDesc, &m_updatedParticles->sBParticlesSRV);
	}

	// Create compute shader particle param structured buffers and unordered access views
	D3D11_BUFFER_DESC paramSBDesc = {};
	paramSBDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	paramSBDesc.Usage = D3D11_USAGE_DEFAULT;
	paramSBDesc.CPUAccessFlags = 0;
	paramSBDesc.StructureByteStride = sizeof(particleDispatchParams);
	paramSBDesc.ByteWidth = sizeof(particleDispatchParams);
	paramSBDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	uavDesc.Format = DXGI_FORMAT_R32_UINT;
	uavDesc.Buffer.NumElements = sizeof(particleDispatchParams) / sizeof(UINT);

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&paramSBDesc, 0, &m_prevParticles->bParams)))
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_prevParticles->bParams, &uavDesc, &m_prevParticles->bParamsUAV);

	if (SUCCEEDED(DX::getInstance()->getDevice()->CreateBuffer(&paramSBDesc, 0, &m_updatedParticles->bParams)))
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_updatedParticles->bParams, &uavDesc, &m_updatedParticles->bParamsUAV);


	// Create compute shader constant buffer, to update delta time
	m_computeShaderConstantBuffer = new ConstantBuffer(&XMVectorSet(0, 0, 0, 0), sizeof(XMVECTOR));
}

Particles::~Particles()
{
	// Shaders
	m_computeShaderParticles.release();
	m_vertexShaderParticles.release();
	m_geometryShaderParticles.release();
	m_pixelShaderParticles.release();

	// Buffers and views
	if (m_computeShaderConstantBuffer) delete m_computeShaderConstantBuffer;
	for (int i = 0; i < 2; i++)
	{
		if (m_actualBuffers[i].bParams) m_actualBuffers[i].bParams->Release();
		if (m_actualBuffers[i].bParamsUAV) m_actualBuffers[i].bParamsUAV->Release();
		if (m_actualBuffers[i].sBParticles) m_actualBuffers[i].sBParticles->Release();
		if (m_actualBuffers[i].sBParticlesUAV) m_actualBuffers[i].sBParticlesUAV->Release();
		if (m_actualBuffers[i].sBParticlesSRV) m_actualBuffers[i].sBParticlesSRV->Release();
	}
}

void Particles::addParticles(XMVECTOR position, XMVECTOR color, XMVECTOR size, int amount, float velocity, XMVECTOR direction)
{
	int nrToAdd = (amount > MAX_ADD || amount <= 0) ? MAX_ADD : amount;
	bool dirGiven = XMVector3Equal(direction, XMVectorSet(0, 0, 0, 0)) ? false : true;

	if (dirGiven) // If direction was given, randomize nrToAdd velocity vectors after direction
	{
		// Calculate direction normal
		float nX = direction.m128_f32[0];
		float nY = direction.m128_f32[1];
		float nZ = direction.m128_f32[2];

		// Calculate direction tangent, rotate direction in two axis to get a non-parallel vector
		XMVECTOR temp = XMVector3Rotate(direction, XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), 1));
		temp = XMVector3Rotate(temp, XMQuaternionRotationNormal(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), 1)); // Bottom
		XMVECTOR tangent = XMVector3Cross(direction, temp);
		float tX = tangent.m128_f32[0];
		float tY = tangent.m128_f32[1];
		float tZ = tangent.m128_f32[2];

		// Calculate direction bitangent
		XMVECTOR bitangent = XMVector3Cross(direction, tangent);
		float bX = bitangent.m128_f32[0];
		float bY = bitangent.m128_f32[1];
		float bZ = bitangent.m128_f32[2];

		// Calculate TBN matrix
		XMMATRIX tBN =
		{
			tX, tY, tZ, 0.0f,
			bX, bY, bZ, 0.0f,
			nX, nY, nZ, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};

		for (int i = 0; i < nrToAdd; i++)
		{
			XMVECTOR randomZVec = XMVector3Normalize(XMVectorSet
			(
				randF(),
				randF(),
				1,
				0
			));
			XMVECTOR vel = XMVector3Transform(randomZVec, tBN);
			//XMVECTOR vel = randomZVec;
			vel *= velocity * (1.0f + 0.4f * randF());
			particle p =
			{
				position.m128_f32[0], position.m128_f32[1], position.m128_f32[2],
				vel.m128_f32[0], vel.m128_f32[1], vel.m128_f32[2],
				size.m128_f32[0], size.m128_f32[1],
				color.m128_f32[0], color.m128_f32[1], color.m128_f32[2]
			};
			m_particleToAdd.push_back(p);
		}
	}
	else // If no direction was given, randomize nrToAdd velocity vectors
	{
		for (int i = 0; i < nrToAdd; i++)
		{
			XMVECTOR vel = XMVector3Normalize(XMVectorSet
			(
				randF(),
				randF(),
				randF(),
				0.0f
			));

			vel *= velocity * (1.0f + 0.4f * randF());
			particle p =
			{
				position.m128_f32[0], position.m128_f32[1], position.m128_f32[2],
				vel.m128_f32[0], vel.m128_f32[1], vel.m128_f32[2],
				size.m128_f32[0], size.m128_f32[1],
				color.m128_f32[0], color.m128_f32[1], color.m128_f32[2]
			};
			m_particleToAdd.push_back(p);
		}
	}

}

void Particles::addSpark(XMVECTOR impactPos, XMVECTOR projectileDir)
{
	XMVECTOR dir = XMVector3Normalize(-projectileDir);
	XMVECTOR col = { 243.0f / 255.0f, 185.0f / 255.0f, 70.0f / 255.0f, 0 };
	XMVECTOR size = { 0.5f, 0.15f, 0.0f, 0.0f };
	addParticles(impactPos, col, size, 10, 5.0f, dir);
}

void Particles::update(float dT)
{
	// Swap prev and updated structured buffer order
	std::swap(m_prevParticles, m_updatedParticles);

	// Update compute shader constant buffer with delta time
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	ID3D11Buffer* tempPtr = *m_computeShaderConstantBuffer->getConstantBuffer();
	DX::getInstance()->getDeviceContext()->Map(tempPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(dT, dT, dT, dT), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(tempPtr, 0);

	// Reset updated param buffer and add new particle params to loop
	particleDispatchParams params;
	UINT nrToAdd = (UINT)m_particleToAdd.size();
	params.draw.vertexCountPerInstance = nrToAdd;
	params.dispatch.threadGroupCountX = (nrToAdd > 0) ? (UINT)1 + nrToAdd / (UINT)128 : 0;
	D3D11_BOX paramBox;
	paramBox.left = 0;
	paramBox.right = sizeof(params);
	paramBox.top = 0;
	paramBox.bottom = 1;
	paramBox.front = 0;
	paramBox.back = 1;
	DX::getInstance()->getDeviceContext()->UpdateSubresource(m_updatedParticles->bParams, 0, &paramBox, &params, 0, 0);

	// Reset updated particle buffer and add new particles to loop
	if (nrToAdd > 0)
	{
		D3D11_BOX particlesBox;
		particlesBox.left = 0;
		particlesBox.right = (UINT)(sizeof(particle) * nrToAdd);
		particlesBox.top = 0;
		particlesBox.bottom = 1;
		particlesBox.front = 0;
		particlesBox.back = 1;
		DX::getInstance()->getDeviceContext()->UpdateSubresource(m_updatedParticles->sBParticles, 0, &particlesBox, m_particleToAdd.data(), 0, 0);
	}

	// Clear particles to add
	m_particleToAdd.clear();

	// Set unordered access views, read from prev and write to updated
	ID3D11UnorderedAccessView* uAV[4] =
	{
		m_prevParticles->bParamsUAV,
		m_prevParticles->sBParticlesUAV,
		m_updatedParticles->bParamsUAV,
		m_updatedParticles->sBParticlesUAV
	};
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, _countof(uAV), uAV, 0);

	// Bind compute shader and constant buffer
	DX::getInstance()->getDeviceContext()->CSSetShader(&m_computeShaderParticles.getComputeShader(), 0, 0);
	DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(0, 1, m_computeShaderConstantBuffer->getConstantBuffer());

	// Run compute shader
	DX::getInstance()->getDeviceContext()->DispatchIndirect(m_prevParticles->bParams, sizeof(drawParameters));

	// Unbind compute shader and constant buffer
	ID3D11Buffer* nullCB = nullptr;
	DX::getInstance()->getDeviceContext()->CSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(0, 1, &nullCB);


	// Unbind UAVs
	ID3D11UnorderedAccessView* emptyUAVs[4] =
	{
		nullptr,
		nullptr,
		nullptr,
		nullptr
	};
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, _countof(emptyUAVs), emptyUAVs, 0);
}

void Particles::draw()
{
	// Disable transparency
	DX::getInstance()->getDeviceContext()->OMSetBlendState(nullptr, NULL, 0xFFFFFFFF);

	// Enable emission
	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSEnabled(), 1);

	// Disable unused shaders
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);

	// Set input assembler
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	DX::getInstance()->getDeviceContext()->IASetInputLayout(nullptr);

	// Set vertex shader
	DX::getInstance()->getDeviceContext()->VSSetShader(&m_vertexShaderParticles.getVertexShader(), nullptr, NULL);
	DX::getInstance()->getDeviceContext()->VSSetShaderResources(0, 1, &m_updatedParticles->sBParticlesSRV);

	// Set geometry shader
	ID3D11Buffer* camInfo[2] =
	{
		*DX::getInstance()->getCam()->getConstantBufferVP()->getConstantBuffer(),
		*DX::getInstance()->getCam()->getConstantBufferPosition()->getConstantBuffer()
	};
	DX::getInstance()->getDeviceContext()->GSSetConstantBuffers(0, 2, camInfo);
	DX::getInstance()->getDeviceContext()->GSSetShader(&m_geometryShaderParticles.getGeometryShader(), nullptr, NULL);


	// Set pixel shader
	DX::getInstance()->getDeviceContext()->PSSetShader(&m_pixelShaderParticles.getPixelShader(), nullptr, NULL);

	// Draw
	DX::getInstance()->getDeviceContext()->DrawInstancedIndirect(m_updatedParticles->bParams, 0); // Read from prev, draw from updated

	// Disable emission
	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(DX::getInstance()->getDSSDisabled(), 1);

	// Disable used shaders
	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX::getInstance()->getDeviceContext()->VSSetShaderResources(0, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShaderResources(0, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}
