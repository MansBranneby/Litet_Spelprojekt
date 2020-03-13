#include "Lights.h"
Lights* Lights::m_instance = nullptr;
Lights::Lights()
{

}

Lights::~Lights()
{
}

int Lights::getFreeIndex()
{
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		if (!m_lights[i].enabled)
			return i;
	}
	return -1;
}

void Lights::setLightData()
{
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		m_lights[i].enabled = false;
		m_lights[i].intensity = 1;
	}


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

void Lights::releaseInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

LightData* Lights::getLights()
{
	return m_lights;
}

int Lights::addPointLight(float x, float y, float z, float radius, float r, float g, float b, float intensity)
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].x = x;
	m_lights[lightIndex].y = y;
	m_lights[lightIndex].z = z;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].type = 0;
	m_lights[lightIndex].r = r;
	m_lights[lightIndex].g = g;
	m_lights[lightIndex].b = b;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].intensity = intensity;
	m_lights[lightIndex].range = radius;
	
	return lightIndex;
}

int Lights::addDirectionalLight(float dx, float dy, float dz, float r, float g, float b, float intensity)
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].dx = dx;
	m_lights[lightIndex].dy = dy;
	m_lights[lightIndex].dz = dz;
	m_lights[lightIndex].type = 1;
	m_lights[lightIndex].r = r;
	m_lights[lightIndex].g = g;
	m_lights[lightIndex].b = b;
	m_lights[lightIndex].a = 1;

	m_lights[lightIndex].intensity = intensity;
	
	return lightIndex;
}

int Lights::addSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity)
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].type = 2;
	m_lights[lightIndex].spotLightAngle = angleDeg;
	m_lights[lightIndex].r = r;
	m_lights[lightIndex].g = g;
	m_lights[lightIndex].b = b;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].x = x;
	m_lights[lightIndex].y = y;
	m_lights[lightIndex].z = z;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].range = range;
	m_lights[lightIndex].dx = dx;
	m_lights[lightIndex].dy = dy;
	m_lights[lightIndex].dz = dz;
	m_lights[lightIndex].intensity = intensity;
	
	
	return lightIndex;
}

int Lights::addAreaLight(float x, float y, float z, float radius, float r, float g, float b, float intensity)
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].x = x;
	m_lights[lightIndex].y = y;
	m_lights[lightIndex].z = z;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].type = 3;
	m_lights[lightIndex].r = r;
	m_lights[lightIndex].g = g;
	m_lights[lightIndex].b = b;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].intensity = intensity;
	m_lights[lightIndex].range = radius;
	
	return lightIndex;
}

int Lights::addVolumetricSpotLight(float x, float y, float z, float range, float dx, float dy, float dz, float r, float g, float b, float angleDeg, float intensity)
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].type = 4;
	m_lights[lightIndex].spotLightAngle = angleDeg;
	m_lights[lightIndex].r = r;
	m_lights[lightIndex].g = g;
	m_lights[lightIndex].b = b;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].x = x;
	m_lights[lightIndex].y = y;
	m_lights[lightIndex].z = z;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].range = range;
	m_lights[lightIndex].dx = dx;
	m_lights[lightIndex].dy = dy;
	m_lights[lightIndex].dz = dz;
	m_lights[lightIndex].intensity = intensity;
	

	return lightIndex;
}

int Lights::addVolumetricSpotLight()
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].type = 4;
	m_lights[lightIndex].spotLightAngle = 0;
	m_lights[lightIndex].r = 0;
	m_lights[lightIndex].g = 0;
	m_lights[lightIndex].b = 0;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].x = 0;
	m_lights[lightIndex].y = 0;
	m_lights[lightIndex].z = 0;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].range = 0;
	m_lights[lightIndex].dx = 0;
	m_lights[lightIndex].dy = -1;
	m_lights[lightIndex].dz = 0;
	m_lights[lightIndex].intensity = 1;
	


	return lightIndex;
}

int Lights::addPointLight()
{
	int lightIndex = getFreeIndex();
	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].x = 0;
	m_lights[lightIndex].y = 0;
	m_lights[lightIndex].z = 0;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].type = 0;
	m_lights[lightIndex].r = 0;
	m_lights[lightIndex].g = 0;
	m_lights[lightIndex].b = 0;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].intensity = 1;
	m_lights[lightIndex].range = 0;
	
	

	return lightIndex;
}

int Lights::addDirectionalLight()
{
	int lightIndex = getFreeIndex();

	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].dx = 0;
	m_lights[lightIndex].dy = 0;
	m_lights[lightIndex].dz = 0;
	m_lights[lightIndex].type = 1;
	m_lights[lightIndex].r = 0;
	m_lights[lightIndex].g = 0;
	m_lights[lightIndex].b = 0;
	m_lights[lightIndex].a = 1;

	m_lights[lightIndex].intensity = 1;
	
	

	return lightIndex;
}

int Lights::addSpotLight()
{
	int lightIndex = getFreeIndex();

	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].type = 2;
	m_lights[lightIndex].spotLightAngle = 0;
	m_lights[lightIndex].r = 0;
	m_lights[lightIndex].g = 0;
	m_lights[lightIndex].b = 0;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].x = 0;
	m_lights[lightIndex].y = 0;
	m_lights[lightIndex].z = 0;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].range = 0;
	m_lights[lightIndex].dx = 0;
	m_lights[lightIndex].dy = -1;
	m_lights[lightIndex].dz = 0;
	m_lights[lightIndex].intensity = 1;
	
	

	return lightIndex;
}

int Lights::addAreaLight()
{
	int lightIndex = getFreeIndex();

	m_lights[lightIndex].enabled = true;
	m_lights[lightIndex].x = 0;
	m_lights[lightIndex].y = 0;
	m_lights[lightIndex].z = 0;
	m_lights[lightIndex].w = 1;
	m_lights[lightIndex].type = 3;
	m_lights[lightIndex].r = 0;
	m_lights[lightIndex].g = 0;
	m_lights[lightIndex].b = 0;
	m_lights[lightIndex].a = 1;
	m_lights[lightIndex].intensity = 1;
	m_lights[lightIndex].range = 0;
	


	return lightIndex;
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

void Lights::disable(int index)
{
	m_lights[index].enabled = false;
}

void Lights::Initialize()
{
	setLightData();
}

