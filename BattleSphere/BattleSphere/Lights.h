#pragma once
#include <DirectXMath.h>
#define LIGHT_COUNT 96

using namespace DirectX;

struct LightData
{
	int enabled;
	int type;
	float range, spotLightAngle;
	float x, y, z, w;
	float dx, dy, dz;
	float intensity;
	float r, g, b, a;
};

class Lights
{
	
private:
	LightData m_lights[LIGHT_COUNT] = { 0 };
	int m_nrOfLights = 0;
	static Lights* m_instance;
	void setLightData();
public:
	static Lights* getInstance();
	LightData* getLights();
	int addPointLight(float x, float y, float z, float radius, float r, float g, float b, float intensity);
	int addDirectionalLight(float dx, float dy, float dz, float r, float g, float b, float intensity);
	int addSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity);
	int addAreaLight(float x, float y, float z, float radius, float r, float g, float b, float intensity);
	int addVolumetricSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity);
	int addVolumetricSpotLight();
	int addPointLight();
	int addDirectionalLight();
	int addSpotLight();
	int addAreaLight();
	void setPosition(int index, float x, float y, float z);
	void setDirection(int index, float dx, float dy, float dz);
	void setColor(int index, float r, float g, float b);
	void setRange(int index, float range);
	void setIntensity(int index, float intensity);
	void setAngle(int index, float angleDeg);
	void Initialize();
	
	Lights();
	~Lights();
};

