#pragma once
#include "StructsAndEnums.h"

#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ConstantBuffer.h"

#define MAX_PARTICLES 512 * 16 *8 // Max groups * Threads per group * computed per thread
#define MAX_ADD 2000

	struct particle
	{
		float posX, posY, posZ;
		float velX, velY, velZ;
		float sizeX, sizeY;
		float timeLived;
		float lifeSpan;
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

	// Randomizer
	float randF(); // Returns float, range [-1, 1]

public:
	Particles();
	~Particles();

	void addParticles(XMVECTOR pos, XMVECTOR color, XMVECTOR size, int amount = 200, float velocity = 3.0f, float lifeSpan = 0.0f, float spread = 1.0f, XMVECTOR direction = { 0.0f, 0.0f, 0.0f, 0.0f });
	void addSpark(XMVECTOR impactPos, XMVECTOR projectileDir);
	void addCutSpark(XMVECTOR cutPos, XMVECTOR sparkDir);
	void update(float dT);
	void draw();
};

