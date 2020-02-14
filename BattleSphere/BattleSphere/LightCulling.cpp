#include "LightCulling.h"

void LightCulling::setLightData()
{
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		m_lights[i].enabled = false;
		m_lights[i].intensity = 1;
	}
	m_nrOfLights = 0;




	
}

void LightCulling::createConstantBuffers()
{
	m_viewMatrixData = new XMMATRIX();
	*m_viewMatrixData = DX::getInstance()->getCam()->getViewMatrix();
	m_viewMatrixCBuffer = new ConstantBuffer(m_viewMatrixData, sizeof(XMMATRIX));

	/*Data for compute shader, gives increased thread information*/
	m_dispatchData = new dispatchInfo();
	m_dispatchData->threadsX = 16 * 4;
	m_dispatchData->threadsY = 16 * 3;
	m_dispatchData->threadsZ = 1;
	m_dispatchData->threadGroupsX = 4;
	m_dispatchData->threadGroupsY = 3;
	m_dispatchData->threadGroupsZ = 1;
	m_dispatchCBuffer = new ConstantBuffer(m_dispatchData, sizeof(dispatchInfo));

	/*Data for compute shader, gives screen information and inverse projection matrix
	to convert from screen space to view space*/
	m_screenToViewData = new screenToViewInfo();
	m_screenToViewData->height = DX::getInstance()->getHeight();
	m_screenToViewData->width = DX::getInstance()->getWidth();
	m_screenToViewData->inverseProj = XMMatrixInverse(nullptr, DX::getInstance()->getCam()->getProjectionMatrix());
	m_screenToViewCBuffer = new ConstantBuffer(m_screenToViewData, sizeof(screenToViewInfo));
}

void LightCulling::createLightData()
{
	m_computeShaderLightCulling = new ComputeShader(L"ComputeShaderLightCulling.hlsl");

	/*Creates structured buffer for lightIndexList and counter*/
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(UINT);
	bufferDesc.ByteWidth = sizeof(UINT) * 1;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, 0, &m_lightIndexCountBuffer);

	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = sizeof(UINT) * INDEX_COUNT;
	DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, 0, &m_lightIndexListBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Buffer.ElementOffset = 0;
	srvDesc.Buffer.ElementWidth = sizeof(UINT);
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = INDEX_COUNT;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	if (m_lightIndexListBuffer != nullptr)
	DX::getInstance()->getDevice()->CreateShaderResourceView(m_lightIndexListBuffer, &srvDesc, &m_lightIndexListSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	if(m_lightIndexCountBuffer != nullptr)
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_lightIndexCountBuffer, &uavDesc, &m_lightIndexCountUAV);
	uavDesc.Buffer.NumElements = INDEX_COUNT;
	if (m_lightIndexListBuffer != nullptr)
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_lightIndexListBuffer, &uavDesc, &m_lightIndexListUAV);


	/*Create lightgrid texture*/
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = (UINT)DX::getInstance()->getWidth() / 32;
	texDesc.Height = (UINT)DX::getInstance()->getHeight() / 32;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDesc, NULL, &m_lightGridTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvTextureDesc;
	srvTextureDesc.Texture2DArray.MostDetailedMip = 0;
	srvTextureDesc.Texture2DArray.MipLevels = 1;
	srvTextureDesc.Texture2DArray.FirstArraySlice = 0;
	srvTextureDesc.Texture2DArray.ArraySize = 1;
	srvTextureDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvTextureDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	if (m_lightGridTexture != nullptr)
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_lightGridTexture, &srvTextureDesc, &m_lightGridSRV);

	uavDesc.Buffer.NumElements = m_dispatchData->threadGroupsX * m_dispatchData->threadGroupsY * m_dispatchData->threadsX * m_dispatchData->threadsY;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	if (m_lightGridTexture != nullptr)
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_lightGridTexture, &uavDesc, &m_lightGridUAV);



	/*Create light data*/
	bufferDesc.StructureByteStride = sizeof(Light);
	bufferDesc.ByteWidth = sizeof(Light) * LIGHT_COUNT;


	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_lights[0];
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, &initData, &m_lightBuffer);


	srvDesc.Buffer.ElementWidth = sizeof(Light);
	srvDesc.Buffer.NumElements = LIGHT_COUNT;
	if (m_lightBuffer != nullptr)
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_lightBuffer, &srvDesc, &m_lightSRV);

	srvDesc.Buffer.ElementWidth = sizeof(frustum);
	srvDesc.Buffer.NumElements = FRUSTUM_COUNT;
	if (m_structuredResultBuffer != nullptr)
		DX::getInstance()->getDevice()->CreateShaderResourceView(m_structuredResultBuffer, &srvDesc, &m_frustumSRV);
	

}

void LightCulling::createFrustumData()
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(frustum);
	bufferDesc.ByteWidth = sizeof(frustum) * FRUSTUM_COUNT;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, 0, &m_structuredBufferOut);


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = FRUSTUM_COUNT;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	if (m_structuredBufferOut != nullptr)
		DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_structuredBufferOut, &uavDesc, &m_structuredBufferUAVOut);

	
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, 0, &m_structuredResultBuffer);
}

void LightCulling::updateSubresource()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_lights, sizeof(Light) * LIGHT_COUNT);
	DX::getInstance()->getDeviceContext()->Unmap(m_lightBuffer, 0);
}

LightCulling::LightCulling()
{
	m_computeShaderFrustum = nullptr;
	m_computeShaderLightCulling = nullptr;
	m_dispatchCBuffer = nullptr;
	m_dispatchData = nullptr;
	m_screenToViewCBuffer = nullptr;
	m_screenToViewData = nullptr;
	m_structuredBufferOut = nullptr;
	m_structuredBufferUAVOut = nullptr;
	m_structuredResultBuffer = nullptr;
	m_lightIndexCountBuffer = nullptr;
	m_lightIndexCountUAV = nullptr;

	m_lightIndexListBuffer = nullptr;
	m_lightIndexListUAV = nullptr;
	m_lightIndexListSRV = nullptr;

	m_lightGridTexture = nullptr;
	m_lightGridUAV = nullptr;
	m_lightGridSRV = nullptr;

	m_viewMatrixData = nullptr;
	m_viewMatrixCBuffer = nullptr;

	m_lightBuffer = nullptr;
	m_lightSRV = nullptr;
	m_frustumSRV = nullptr;

}

LightCulling::~LightCulling()
{
	if (m_computeShaderFrustum)
	{
		m_computeShaderFrustum->release();
		delete m_computeShaderFrustum;
		m_computeShaderFrustum = nullptr;
	}
	if (m_computeShaderLightCulling)
	{
		m_computeShaderLightCulling->release();
		delete m_computeShaderLightCulling;
		m_computeShaderLightCulling = nullptr;
	}
	
	if (m_dispatchData)
	{
		delete m_dispatchData;
		m_dispatchData = nullptr;
	}
	if (m_dispatchCBuffer)
	{
		delete m_dispatchCBuffer;
		m_dispatchCBuffer = nullptr;
	}
	if (m_screenToViewData)
	{
		delete m_screenToViewData;
		m_screenToViewData = nullptr;
	}
	if (m_screenToViewCBuffer)
	{
		delete m_screenToViewCBuffer;
		m_screenToViewCBuffer = nullptr;
	}

	if (m_lightIndexCountBuffer)
	{
		m_lightIndexCountBuffer->Release();
		m_lightIndexCountBuffer = nullptr;
	}
	if (m_lightIndexCountUAV)
	{
		m_lightIndexCountUAV->Release();
		m_lightIndexCountUAV = nullptr;
	}
	if (m_structuredBufferOut)
	{
		m_structuredBufferOut->Release();
		m_structuredBufferOut = nullptr;
	}
	if (m_structuredBufferUAVOut)
	{
		m_structuredBufferUAVOut->Release();
		m_structuredBufferUAVOut = nullptr;
	}
	if (m_structuredResultBuffer)
	{
		m_structuredResultBuffer->Release();
		m_structuredResultBuffer = nullptr;
	}
	if (m_lightIndexListBuffer) 
	{
		m_lightIndexListBuffer->Release();
		m_lightIndexListBuffer = nullptr;
	}
	if (m_lightIndexListUAV) 
	{
		m_lightIndexListUAV->Release();
		m_lightIndexListUAV = nullptr;
	}
	if (m_lightIndexListSRV) 
	{
		m_lightIndexListSRV->Release();
		m_lightIndexListSRV = nullptr;
	}
	if (m_lightGridTexture)
	{
		m_lightGridTexture->Release();
		m_lightGridTexture = nullptr;
	}
	if (m_lightGridUAV)
	{
		m_lightGridUAV->Release();
		m_lightGridUAV = nullptr;
	}
	if (m_lightGridSRV)
	{
		m_lightGridSRV->Release();
		m_lightGridSRV = nullptr;
	}

	if (m_viewMatrixData) 
	{
		delete m_viewMatrixData;
		m_viewMatrixData = nullptr;
	}
	if (m_viewMatrixCBuffer) 
	{
		delete m_viewMatrixCBuffer;
		m_viewMatrixCBuffer = nullptr;
	}

	if (m_lightBuffer) 
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}
	if (m_lightSRV) 
	{
		m_lightSRV->Release();
		m_lightSRV = nullptr;
	}
	if (m_frustumSRV) 
	{
		m_frustumSRV->Release();
		m_frustumSRV = nullptr;
	}
}

int LightCulling::addPointLight(float x, float y, float z, float radius, float r, float g, float b, float intensity)
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].x = x;
	m_lights[m_nrOfLights].y = y;
	m_lights[m_nrOfLights].z = z;
	m_lights[m_nrOfLights].w = 1;
	m_lights[m_nrOfLights].type = 0;
	m_lights[m_nrOfLights].r = r;
	m_lights[m_nrOfLights].g = g;
	m_lights[m_nrOfLights].b = b;
	m_lights[m_nrOfLights].a = 1;
	m_lights[m_nrOfLights].intensity = intensity;
	m_lights[m_nrOfLights].range = radius;
	m_nrOfLights++;
	return m_nrOfLights - 1;
}

int LightCulling::addDirectionalLight(float dx, float dy, float dz, float r, float g, float b, float intensity)
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].dx = dx;
	m_lights[m_nrOfLights].dy = dy;
	m_lights[m_nrOfLights].dz = dz;
	m_lights[m_nrOfLights].type = 1;
	m_lights[m_nrOfLights].r = r;
	m_lights[m_nrOfLights].g = g;
	m_lights[m_nrOfLights].b = b;
	m_lights[m_nrOfLights].a = 1;

	m_lights[m_nrOfLights].intensity = intensity;
	m_nrOfLights++;
	return m_nrOfLights - 1;
}

int LightCulling::addSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity)
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].type = 2;
	m_lights[m_nrOfLights].spotLightAngle = angleDeg;
	m_lights[m_nrOfLights].r = r;
	m_lights[m_nrOfLights].g = b;
	m_lights[m_nrOfLights].b = g;
	m_lights[m_nrOfLights].a = 1;
	m_lights[m_nrOfLights].x = x;
	m_lights[m_nrOfLights].y = y;
	m_lights[m_nrOfLights].z = z;
	m_lights[m_nrOfLights].w = 1;
	m_lights[m_nrOfLights].range = range;
	m_lights[m_nrOfLights].dx = dx;
	m_lights[m_nrOfLights].dy = dy;
	m_lights[m_nrOfLights].dz = dz;
	m_lights[m_nrOfLights].intensity = intensity;
	m_nrOfLights++;
	updateSubresource();
	return m_nrOfLights - 1;
}

int LightCulling::addPointLight()
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].x = 0;
	m_lights[m_nrOfLights].y = 0;
	m_lights[m_nrOfLights].z = 0;
	m_lights[m_nrOfLights].w = 1;
	m_lights[m_nrOfLights].type = 0;
	m_lights[m_nrOfLights].r = 0;
	m_lights[m_nrOfLights].g = 0;
	m_lights[m_nrOfLights].b = 0;
	m_lights[m_nrOfLights].a = 1;
	m_lights[m_nrOfLights].intensity = 1;
	m_lights[m_nrOfLights].range = 0;
	m_nrOfLights++;
	updateSubresource();

	return m_nrOfLights - 1;
}

int LightCulling::addDirectionalLight()
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].dx = 0;
	m_lights[m_nrOfLights].dy = 0;
	m_lights[m_nrOfLights].dz = 0;
	m_lights[m_nrOfLights].type = 1;
	m_lights[m_nrOfLights].r = 0;
	m_lights[m_nrOfLights].g = 0;
	m_lights[m_nrOfLights].b = 0;
	m_lights[m_nrOfLights].a = 1;

	m_lights[m_nrOfLights].intensity = 1;
	m_nrOfLights++;
	updateSubresource();

	return m_nrOfLights - 1;
}

int LightCulling::addSpotLight()
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].type = 2;
	m_lights[m_nrOfLights].spotLightAngle = 0;
	m_lights[m_nrOfLights].r = 0;
	m_lights[m_nrOfLights].g = 0;
	m_lights[m_nrOfLights].b = 0;
	m_lights[m_nrOfLights].a = 1;
	m_lights[m_nrOfLights].x = 0;
	m_lights[m_nrOfLights].y = 0;
	m_lights[m_nrOfLights].z = 0;
	m_lights[m_nrOfLights].w = 1;
	m_lights[m_nrOfLights].range = 0;
	m_lights[m_nrOfLights].dx = 0;
	m_lights[m_nrOfLights].dy = -1;
	m_lights[m_nrOfLights].dz = 0;
	m_lights[m_nrOfLights].intensity = 1;
	m_nrOfLights++;
	updateSubresource();

	return m_nrOfLights - 1;
}

void LightCulling::setPosition(int index, float x, float y, float z)
{
	m_lights[index].x = x;
	m_lights[index].y = y;
	m_lights[index].z = z;
	updateSubresource();

}

void LightCulling::setColor(int index, float r, float g, float b)
{
	m_lights[index].r = r;
	m_lights[index].g = g;
	m_lights[index].b = b;
	updateSubresource();

}

void LightCulling::setRange(int index, float range)
{
	m_lights[index].range = range;
	updateSubresource();

}

void LightCulling::setIntensity(int index, float intensity)
{
	m_lights[index].intensity = intensity;
	updateSubresource();

}

void LightCulling::setAngle(int index, float angleDeg)
{
	m_lights[index].spotLightAngle = angleDeg;
	updateSubresource();

}


void LightCulling::initialize()
{
	m_computeShaderFrustum = new ComputeShader(L"ComputeShaderFrustumCalc.hlsl");
	createConstantBuffers();
	createFrustumData();
	createLightData();



	setLightData();
}

void LightCulling::computeFrustum()
{
	DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(0, 1, m_dispatchCBuffer->getConstantBuffer());
	DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(1, 1, m_screenToViewCBuffer->getConstantBuffer());
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_structuredBufferUAVOut, 0);
	DX::getInstance()->getDeviceContext()->CSSetShader(&m_computeShaderFrustum->getComputeShader(), NULL, 0);
	DX::getInstance()->getDeviceContext()->Dispatch(m_dispatchData->threadGroupsX, m_dispatchData->threadGroupsY, m_dispatchData->threadGroupsZ);
	DX::getInstance()->getDeviceContext()->CopyResource(m_structuredResultBuffer, m_structuredBufferOut);


	
}

void LightCulling::cullLights()
{
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	ID3D11ShaderResourceView* nullSRV = nullptr;
 	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(0, 1, m_dispatchCBuffer->getConstantBuffer());
	DX::getInstance()->getDeviceContext()->CSSetConstantBuffers(1, 1, m_viewMatrixCBuffer->getConstantBuffer());
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_lightIndexCountUAV, 0);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(1, 1, &m_lightIndexListUAV, 0);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(2, 1, &m_lightGridUAV, 0);
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &m_frustumSRV);
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(1, 1, &m_lightSRV);
	DX::getInstance()->getDeviceContext()->CSSetShader(&m_computeShaderLightCulling->getComputeShader(), NULL, 0);
	DX::getInstance()->getDeviceContext()->Dispatch(64, 48, 1);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(1, 1, &nullUAV, 0);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(2, 1, &nullUAV, 0);
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &m_lightGridSRV);
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(1, 1, &m_lightIndexListSRV);
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(2, 1, &m_lightSRV);

	
}