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

	void Initialise();
	void Shutdown();

	bool Frame();

	// Getters
	int GetFPS();
	int GetUsage();
	float GetDeltaTime();

private:
	// FPS
	int FPS_;
	int FrameCount_;
	unsigned long FPSStart_;

	// CPU
	bool CanReadCPU_;
	HQUERY QueryHandle_;
	HCOUNTER CounterHandle_;
	unsigned long LastSampleTime_;
	long Usage_;

	// Timer
	INT64 Frequency_;
	float TicksPerMs_;
	INT64 TimerStart_;
	float FrameTime_;
};


