#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "DX.h"

using namespace DirectX;

class Clock
{
private:
	LARGE_INTEGER m_clockFreq;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_currTime;
	LARGE_INTEGER m_delta;
	float m_dt;
	float m_timeInSec;

public:
	Clock();

	void resetStartTimer();
	void resetSecTimer();
	void calcDeltaTime();
	float getDeltaTime();
	float getTimeInSec();
};