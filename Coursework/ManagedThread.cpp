#include "ManagedThread.h"

ManagedThread::ManagedThread()
{
}

ManagedThread::~ManagedThread()
{
	ForceClose();
}

void ManagedThread::SetFunction(function<void()> function)
{
	workerFunction = function;
}

void ManagedThread::Start(bool isLooped)
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

bool ManagedThread::TryJoin()
{
	if (isFinished)
	{
		if (workerThread.joinable())
		{
			workerThread.join();
			return true;
		}
	}

	return false;
}

void ManagedThread::ForceClose()
{
	isFinished = true;
	TryJoin();
}