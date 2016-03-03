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
	Count_ = 0;
	StartTime_ = timeGetTime();

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
	LastSampleTime = GetTickCount();
	Usage = 0;

	//=======
	// Timer
	//=======

	// Check to see if this WindowManager supports high PerformanceManager timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency_);

	// Find out how many times the frequency counter ticks every millisecond
	TicksPerMs_ = (float)(Frequency_ / 1000);

	// Get the WindowManager time
	QueryPerformanceCounter((LARGE_INTEGER*)&StartTime_);

	// Store the time we initialised
	QueryPerformanceCounter((LARGE_INTEGER*)&BootTime_);
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
	PDH_FMT_COUNTERVALUE value;

	//=====
	// FPS 
	//=====

	Count_++;

	// If one second has passed then update the frame per second speed.
	if (timeGetTime() >= (StartTime_ + 1000))
	{
		FPS_ = Count_;
		Count_ = 0;

		StartTime_ = timeGetTime();
	}

	//=====
	// CPU
	//=====

	if (CanReadCPU_)
	{
		// If it has been 1 second then update the current cpu usage and reset the 1 second timer again.
		if ((LastSampleTime + 1000) < GetTickCount())
		{
			LastSampleTime = GetTickCount();

			PdhCollectQueryData(QueryHandle_);

			PdhGetFormattedCounterValue(CounterHandle_, PDH_FMT_LONG, NULL, &value);

			Usage = value.longValue;
		}
	}

	//=======
	// Timer
	//=======

	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime_);

	// Calculate the difference in time since the last time we queried for the current time.
	timeDifference = (float)(CurrentTime_ - StartTime_);

	// Calculate the frame time by the time difference over the timer speed resolution.
	FrameTime_ = timeDifference / TicksPerMs_;

	// Restart the timer.
	StartTime_ = (unsigned long)CurrentTime_;

	return true;
}

int PerformanceManager::GetUsage()
{
	// If the class can read the cpu from the operating WindowManager then return the current usage.  If not then return zero.
	if (CanReadCPU_)
	{
		return (int)Usage;
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

INT64 PerformanceManager::GetWindowManagerTime()
{
	return CurrentTime_;
}

float PerformanceManager::GetElapsedTime()
{
	return (float)(CurrentTime_ - BootTime_);
}