#include "PerformanceManager.h"

PerformanceManager::PerformanceManager()
{
}
PerformanceManager::~PerformanceManager()
{

}

void PerformanceManager::Initialise()
{
	//=====
	// FPS
	//=====

	FPS_ = 0;
	FrameCount_ = 0;
	FPSStart_ = timeGetTime();

	//======
	// CPU
	//======

	PDH_STATUS status;

	// Initialise the flag indicating whether this object can read the WindowManager cpu usage or not.
	CanReadCPU_ = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &QueryHandle_);
	if (status != ERROR_SUCCESS)
	{
		CanReadCPU_ = false;
	}

	// Set query object to poll all cpus in the WindowManager.
	status = PdhAddCounter(QueryHandle_, TEXT("\\Processor(_Total)\\% processor time"), 0, &CounterHandle_);
	if (status != ERROR_SUCCESS)
	{
		CanReadCPU_ = false;
	}

	// Initialise the start time and cpu usage.
	LastSampleTime_ = GetTickCount();
	Usage_ = 0;

	//=======
	// Timer
	//=======

	// Check to see if this WindowManager supports high PerformanceManager timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency_);

	// Find out how many times the frequency counter ticks every millisecond
	TicksPerMs_ = (float)(Frequency_ / 1000);

	// Get the WindowManager time
	QueryPerformanceCounter((LARGE_INTEGER*)&TimerStart_);
}

void PerformanceManager::Shutdown()
{
	if (CanReadCPU_)
	{
		PdhCloseQuery(QueryHandle_);
	}
}

bool PerformanceManager::Frame()
{
	float timeDifference;
	INT64 currentTime;
	PDH_FMT_COUNTERVALUE value;

	//=====
	// FPS 
	//=====

	FrameCount_++;

	// If one second has passed then update the frame per second speed.
	if (timeGetTime() >= (FPSStart_ + 1000))
	{
		// Store FPS Count
		FPS_ = FrameCount_;

		// Reset
		FrameCount_ = 0;
		FPSStart_ = timeGetTime();
	}

	//=====
	// CPU
	//=====

	if (CanReadCPU_)
	{
		// If it has been 1 second then update the current cpu usage and reset the 1 second timer again.
		if ((LastSampleTime_ + 1000) < GetTickCount())
		{
			LastSampleTime_ = GetTickCount();

			PdhCollectQueryData(QueryHandle_);

			PdhGetFormattedCounterValue(CounterHandle_, PDH_FMT_LONG, NULL, &value);

			Usage_ = value.longValue;
		}
	}

	//=======
	// Timer
	//=======

	// Query the current time
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate difference in time since last check
	timeDifference = (float)(currentTime - TimerStart_);

	// Calculate the frame time by the time difference over the timer speed resolution.
	FrameTime_ = timeDifference / TicksPerMs_;

	// Restart the timer.
	TimerStart_ = (unsigned long)currentTime;

	return true;
}

int PerformanceManager::GetUsage()
{
	// If the class can read the cpu from the operating WindowManager then return the current usage.  If not then return zero.
	if (CanReadCPU_)
	{
		return (int)Usage_;
	}

	return 0;
}

int PerformanceManager::GetFPS()
{
	return FPS_;
}

float PerformanceManager::GetTime()
{
	return FrameTime_;
}