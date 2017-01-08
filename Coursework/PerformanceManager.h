#pragma once

#pragma comment(lib, "pdh.lib")

#include <pdh.h>
#include <windows.h>

#include "Singleton.h"

class PerformanceManager : public Singleton<PerformanceManager>
{
public:
	PerformanceManager();
	~PerformanceManager();

	void initialise();
	void terminate();

	bool update();

	// getters
	int getFPS();
	int getUsage();
	float getDeltaTime();

private:
	// FPS
	int m_fps;
	int m_frameCount;
	unsigned long m_fpsStart;

	// CPU
	bool m_canAccessCPU;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_usage;

	// Timer
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_timerStart;
	float m_frameTime;
};


