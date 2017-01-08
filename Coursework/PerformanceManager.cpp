#include "PerformanceManager.h"

PerformanceManager::PerformanceManager()
{
}
PerformanceManager::~PerformanceManager()
{

}

void PerformanceManager::initialise()
{
	//=====
	// FPS
	//=====

	m_fps = 0;
	m_frameCount = 0;
	m_fpsStart = timeGetTime();

	//======
	// CPU
	//======

	PDH_STATUS status;

	// initialise the flag indicating whether this object can read the system cpu usage or not.
	m_canAccessCPU = true;

	// create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canAccessCPU = false;
	}

	// set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canAccessCPU = false;
	}

	// initialise the start time and cpu usage.
	m_lastSampleTime = GetTickCount();
	m_usage = 0;

	//=======
	// Timer
	//=======

	// Check to see if this system supports high performance timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);

	// Find out how many times the frequency counter ticks every millisecond
	m_ticksPerMs = (float)(m_frequency / 1000);

	// get the system time
	QueryPerformanceCounter((LARGE_INTEGER*)&m_timerStart);
}

void PerformanceManager::terminate()
{
	if (m_canAccessCPU)
	{
		PdhCloseQuery(m_queryHandle);
	}
}

#include "Utilities.h"

bool PerformanceManager::update()
{
	float timeDifference;
	INT64 currentTime;
	PDH_FMT_COUNTERVALUE value;

	//=====
	// FPS 
	//=====

	m_frameCount++;

	// If one second has passed then update the frame per second speed.
	if (timeGetTime() >= (m_fpsStart + 1000))
	{
		// Store FPS Count
		m_fps = m_frameCount;

		// Reset
		m_frameCount = 0;
		m_fpsStart = timeGetTime();
	}

	//=====
	// CPU
	//=====

	if (m_canAccessCPU)
	{
		// If it has been 1 second then update the current cpu usage and reset the 1 second timer again.
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_usage = value.longValue;
		}
	}

	//=======
	// Timer
	//=======

	// Query the current time
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate difference in time since last check
	timeDifference = (float)(currentTime - m_timerStart);

	// Calculate the frame time by the time difference over the timer speed resolution.
	m_frameTime = timeDifference / m_ticksPerMs;

	// Restart the timer.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_timerStart);

	return true;
}

int PerformanceManager::getUsage()
{
	// If the class can read the cpu from the operating system then return the current usage.  If not then return zero.
	if (m_canAccessCPU)
	{
		return (int)m_usage;
	}

	return 0;
}

int PerformanceManager::getFPS()
{
	return m_fps;
}

float PerformanceManager::getDeltaTime()
{
	return m_frameTime / 1000.0f;
}