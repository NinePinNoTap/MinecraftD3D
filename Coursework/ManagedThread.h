#pragma once

#include <functional>
#include <thread>

using namespace std;

template <typename T>
class ManagedThread
{
public:
	ManagedThread()
	{
	}

	~ManagedThread()
	{
	}

	inline void SetFunction(function<T> function) { workerFunction = function; }

	inline void Start(bool isLooped = false)
	{
		isFinished = false;

		// Create a thread
		workerThread = thread([this, isLooped]()
		{
			// Check if we want 
			if (isLooped)
			{
				while (!isFinished)
				{
					workerFunction();
				}
			}
			else
			{
				// Call the function
				workerFunction();

				// Flag its complete
				isFinished = true;
			}
		});
	}

	inline bool TryJoin()
	{
		if (isFinished && workerThread.joinable())
		{
			workerThread.join();
			return true;
		}

		return false;
	}

	inline void ForceClose()
	{
		isFinished = true;
		TryJoin();
	}

private:
	function<T> workerFunction;
	thread workerThread;

	bool isFinished;
};