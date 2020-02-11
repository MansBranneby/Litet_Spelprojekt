#pragma once
#include <DirectXMath.h>
#include "ComputeShader.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include <stdlib.h>
#include <time.h>

#define FRUSTUM_COUNT 14000
#define LIGHT_COUNT 1024
#define INDEX_COUNT 3000000
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
	struct Light 
	{
		int enabled;
		int type;
		float range, spotLightAngle;
		float x, y, z, w;
		float dx, dy, dz;
		float intensity;
		float r, g, b, a;
	};
	Light m_lights[LIGHT_COUNT] = { 0 };
	int m_nrOfLights = 0;
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
	void updateSubresource();
	
public:
	LightCulling();
	~LightCulling();
	int addPointLight(float x, float y, float z, float radius, float r, float g, float b, float intensity);
	int addDirectionalLight(float dx, float dy, float dz, float r, float g, float b, float intensity);
	int addSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity);
	int addPointLight();
	int addDirectionalLight();
	int addSpotLight();
	void setPosition(int index, float x, float y, float z);
	void setColor(int index, float r, float g, float b);
	void setRange(int index, float range);
	void setIntensity(int index, float intensity);
	void setAngle(int index, float angleDeg);
	void initialize();
	void computeFrustum();
	void cullLights();
	void setLightData();
	
};

