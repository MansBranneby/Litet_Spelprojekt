#pragma once
#include "StructsAndEnums.h"
#define MAX_PARTICLES 512 * 16 *8 // Max groups * Threads per group * computed per thread
#define MAX_ADD 1000

	struct particle
	{
		float posX, posY, posZ;
		float velX, velY, velZ;
		float sizeX, sizeY;
		float colorX, colorY, colorZ;
	};

class Particles
{
private:
	struct drawParameters
	{
		UINT vertexCountPerInstance = 0;
		UINT instanceCount = 1;
		UINT startVertexLocation = 0;
		UINT startInstanceLocation = 0;
	};

	struct dispatchParameters
	{
		UINT threadGroupCountX = 0;
		UINT threadGroupCountY = 1;
		UINT threadGroupCountZ = 1;
	};

	struct particleDispatchParams
	{
		drawParameters draw;
		dispatchParameters dispatch;
	};

	struct particleBuffers
	{
		ID3D11Buffer* bParams = nullptr;
		ID3D11UnorderedAccessView* bParamsUAV = nullptr;

		ID3D11Buffer* sBParticles = nullptr;
		ID3D11UnorderedAccessView* sBParticlesUAV = nullptr;
		ID3D11ShaderResourceView* sBParticlesSRV = nullptr;
	};

	// Compute shader
	ComputeShader m_computeShaderParticles;
	ConstantBuffer* m_computeShaderConstantBuffer;
	particleBuffers* m_prevParticles;
	particleBuffers* m_updatedParticles;
	particleBuffers m_actualBuffers[2];
	std::vector<particle> m_particleToAdd;

	// Vertex shader
	VertexShader m_vertexShaderParticles;

	// Geometry shader
	GeometryShader m_geometryShaderParticles;

	// Pixel shader
	PixelShader m_pixelShaderParticles;

public:
	Particles();
	~Particles();

	void addParticles(XMVECTOR position, XMVECTOR color, XMVECTOR size, int amount = 200, float velocity = 3.0f, XMVECTOR direction = { 0.0f, 0.0f, 0.0f, 0.0f });
	void update(float dT); // Update time constant buffer and view proj matrices
	void draw();
};

