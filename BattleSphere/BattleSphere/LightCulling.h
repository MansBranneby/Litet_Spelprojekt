#pragma once
#include <DirectXMath.h>
#include "ComputeShader.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include <stdlib.h>
#include <time.h>
#include "Lights.h"

#define FRUSTUM_COUNT 14000
#define INDEX_COUNT FRUSTUM_COUNT * LIGHT_COUNT
class LightCulling
{
private:
	struct dispatchInfo 
	{
		UINT threadGroupsX, threadGroupsY, threadGroupsZ;
		UINT padding;

		UINT threadsX, threadsY, threadsZ;
		UINT padding2;
	};
	struct screenToViewInfo 
	{
		XMMATRIX inverseProj;
		float width, height, padding, padding2;
	};
	struct plane 
	{
		float nx, ny, nz;
		float d;
	};
	struct frustum 
	{
		plane planes[4] = { 0 };
	};
	
	Lights* m_lights;
	ID3D11Buffer* m_lightIndexCountBuffer;
	ID3D11UnorderedAccessView* m_lightIndexCountUAV;

	ID3D11Buffer* m_lightIndexListBuffer;
	ID3D11UnorderedAccessView* m_lightIndexListUAV;
	ID3D11ShaderResourceView* m_lightIndexListSRV;

	ID3D11Texture2D* m_lightGridTexture;
	ID3D11UnorderedAccessView* m_lightGridUAV;
	ID3D11ShaderResourceView* m_lightGridSRV;

	XMMATRIX* m_viewMatrixData;
	ConstantBuffer* m_viewMatrixCBuffer;

	ID3D11Buffer* m_lightBuffer;
	ID3D11ShaderResourceView* m_lightSRV;
	ID3D11ShaderResourceView* m_frustumSRV;

	ComputeShader* m_computeShaderFrustum;
	ComputeShader* m_computeShaderLightCulling;

	ConstantBuffer* m_dispatchCBuffer;
	ConstantBuffer* m_screenToViewCBuffer;


	dispatchInfo* m_dispatchData;
	screenToViewInfo* m_screenToViewData;

	ID3D11Buffer* m_structuredBufferOut;
	ID3D11UnorderedAccessView* m_structuredBufferUAVOut;
	ID3D11Buffer* m_structuredResultBuffer;
	void createConstantBuffers();
	void createLightData();
	void createFrustumData();

	
public:
	void updateSubresource();
	LightCulling();
	~LightCulling();
	void initialize();
	void computeFrustum();
	void cullLights();

};

