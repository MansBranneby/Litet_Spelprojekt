#pragma once
#include "StructsAndEnums.h"

class Particles
{
private:
	struct particle
	{
		XMVECTOR position;
		XMVECTOR velocity;
	};

	// List of lists of particles, each with separate dispatch, 
	// along with color and size constant buffers
	std::vector<std::vector<XMVECTOR>> particleGroups;
	std::vector<XMVECTOR> particleColors;
	std::vector<XMVECTOR> particleSizes;

	// Compute shader
	ComputeShader m_computeShaderParticles;
	ConstantBuffer m_computeSizeConstantBuffer;
	ID3D11Buffer* m_structuredBufferOut;

	// Vertex shader
	VertexShader m_vertexShaderParticles;

	// Geometry shader
	GeometryShader m_geometryShaderParticles;

	// Pixel shader
	PixelShader m_pixelShaderParticles;

public:
	Particles();
	~Particles();

	void addParticles(XMVECTOR position, XMVECTOR color, XMVECTOR size, int amount = 200, float velocity = 3.0f);
	void update(float dT);
	void draw();
};

