#pragma once
#include "StructsAndEnums.h"
#define MAX_PARTICLES 8192*8 // Maximum threads * amount computed per thread 
#define MAX_ADD 1000

class Particles
{
private:
	struct particle
	{
		float posX, posY, posZ;
		float velX, velY, velZ;
		float sizeX, sizeY;
		float colorX, colorY, colorZ;
	};

	struct newParticleGroup
	{
		particle particles[MAX_ADD];
	};
	struct newParticleGroupParams
	{
		UINT drawParams[4];
		UINT ThreadGroupCount[3];
	};

	// Compute shader
	ComputeShader m_computeShaderParticles;
	ConstantBuffer* m_computeShaderConstantBuffer;
	ID3D11Buffer* m_structuredBufferCSPingParam;
	ID3D11Buffer* m_structuredBufferCSPing;
	ID3D11Buffer* m_structuredBufferCSPongParam;
	ID3D11Buffer* m_structuredBufferCSPong;
	ID3D11UnorderedAccessView* m_structuredBufferUAVCS[2]; // Ping and pong view
	int m_readIndex;

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

