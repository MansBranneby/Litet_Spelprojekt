#include "Lights.h"
Lights* Lights::m_instance = nullptr;
Lights::Lights()
{

}

Lights::~Lights()
{
}

void Lights::setLightData()
{
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		m_lights[i].enabled = false;
		m_lights[i].intensity = 1;
	}
	m_nrOfLights = 0;

}

Lights* Lights::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Lights();
		m_instance->Initialize();
	}
		
	return m_instance;
}

LightData* Lights::getLights()
{
	return m_lights;
}

int Lights::addPointLight(float x, float y, float z, float radius, float r, float g, float b, float intensity)
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

int Lights::addDirectionalLight(float dx, float dy, float dz, float r, float g, float b, float intensity)
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

int Lights::addSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity)
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].type = 2;
	m_lights[m_nrOfLights].spotLightAngle = angleDeg;
	m_lights[m_nrOfLights].r = r;
	m_lights[m_nrOfLights].g = g;
	m_lights[m_nrOfLights].b = b;
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
	
	return m_nrOfLights - 1;
}

int Lights::addAreaLight(float x, float y, float z, float radius, float r, float g, float b, float intensity)
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].x = x;
	m_lights[m_nrOfLights].y = y;
	m_lights[m_nrOfLights].z = z;
	m_lights[m_nrOfLights].w = 1;
	m_lights[m_nrOfLights].type = 3;
	m_lights[m_nrOfLights].r = r;
	m_lights[m_nrOfLights].g = g;
	m_lights[m_nrOfLights].b = b;
	m_lights[m_nrOfLights].a = 1;
	m_lights[m_nrOfLights].intensity = intensity;
	m_lights[m_nrOfLights].range = radius;
	m_nrOfLights++;
	return m_nrOfLights - 1;
}

int Lights::addVolumetricSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity)
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].type = 4;
	m_lights[m_nrOfLights].spotLightAngle = angleDeg;
	m_lights[m_nrOfLights].r = r;
	m_lights[m_nrOfLights].g = g;
	m_lights[m_nrOfLights].b = b;
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

	return m_nrOfLights - 1;
}

int Lights::addVolumetricSpotLight()
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].type = 4;
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


	return m_nrOfLights - 1;
}

int Lights::addPointLight()
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
	

	return m_nrOfLights - 1;
}

int Lights::addDirectionalLight()
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
	

	return m_nrOfLights - 1;
}

int Lights::addSpotLight()
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
	

	return m_nrOfLights - 1;
}

int Lights::addAreaLight()
{
	m_lights[m_nrOfLights].enabled = true;
	m_lights[m_nrOfLights].x = 0;
	m_lights[m_nrOfLights].y = 0;
	m_lights[m_nrOfLights].z = 0;
	m_lights[m_nrOfLights].w = 1;
	m_lights[m_nrOfLights].type = 3;
	m_lights[m_nrOfLights].r = 0;
	m_lights[m_nrOfLights].g = 0;
	m_lights[m_nrOfLights].b = 0;
	m_lights[m_nrOfLights].a = 1;
	m_lights[m_nrOfLights].intensity = 1;
	m_lights[m_nrOfLights].range = 0;
	m_nrOfLights++;


	return m_nrOfLights - 1;
}

void Lights::setPosition(int index, float x, float y, float z)
{
	m_lights[index].x = x;
	m_lights[index].y = y;
	m_lights[index].z = z;
}

void Lights::setDirection(int index, float dx, float dy, float dz)
{
	m_lights[index].dx = dx;
	m_lights[index].dy = dy;
	m_lights[index].dz = dz;
}

void Lights::setColor(int index, float r, float g, float b)
{
	m_lights[index].r = r;
	m_lights[index].g = g;
	m_lights[index].b = b;
	

}

void Lights::setRange(int index, float range)
{
	m_lights[index].range = range;
	

}

void Lights::setIntensity(int index, float intensity)
{
	m_lights[index].intensity = intensity;
	

}

void Lights::setAngle(int index, float angleDeg)
{
	m_lights[index].spotLightAngle = angleDeg;
	

}

void Lights::Initialize()
{
	setLightData();
}

