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
	int GetUsage();
	int GetFPS();
	float GetTime();
	INT64 GetWindowManagerTime();
	float GetElapsedTime();

private:
	// FPS
	int FPS_;
	int Count_;
	unsigned long FPSCheckStartTime_;

	// CPU
	bool CanReadCPU_;
	HQUERY QueryHandle_;
	HCOUNTER CounterHandle_;
	unsigned long LastSampleTime;
	long Usage;

	// Timer
	INT64 Frequency_;
	float TicksPerMs_;
	INT64 CurrentTime_;
	INT64 BootTime_;
	float FrameTime_;
	unsigned long TimerStartTime_;
};


