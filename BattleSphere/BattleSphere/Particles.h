#pragma once
#include "StructsAndEnums.h"
#define MAX_PARTICLES 10000

class Particles
{
private:
	struct particle
	{
		XMVECTOR position;
		XMVECTOR velocity;
	};

	struct computeShaderData
	{
		float xSize;
		float ySize;
		float dT;
		float padding;
	};

	// List of lists of particles, each with separate dispatch, 
	// along with color and size constant buffers
	std::vector<std::vector<XMVECTOR>> particleGroups;
	std::vector<XMVECTOR> particleColors;
	std::vector<XMVECTOR> particleSizes;

	// Compute shader
	ComputeShader m_computeShaderParticles;
	ConstantBuffer* m_computeShaderConstantBuffer;
	ID3D11Buffer* m_structuredBufferCSWrite;
	ID3D11Buffer* m_structuredBufferCSRead;
	ID3D11UnorderedAccessView* m_structuredBufferUAVCS; // [0] = in(written), [1] = out(to be read)

	// Vertex shader
	ID3D11ShaderResourceView* m_structuredBufferSRV;
	VertexShader m_vertexShaderParticles;
	ConstantBuffer* m_vertexShaderConstantBuffer;

	// Geometry shader
	GeometryShader m_geometryShaderParticles;

	// Pixel shader
	PixelShader m_pixelShaderParticles;
	ConstantBuffer* m_pixelShaderConstantBuffer;

public:
	Particles();
	~Particles();

	void addParticles(XMVECTOR position, XMVECTOR color, XMVECTOR size, int amount = 200, float velocity = 3.0f, XMVECTOR direction = {0.0f, 0.0f, 0.0f, 0.0f});
	void update(float dT);
	void draw();
};

