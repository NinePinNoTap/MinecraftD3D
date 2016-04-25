#pragma once



struct ChunkThread
{
	ChunkThread()
	{
	}

	~ChunkThread()
	{
		TryJoin();
	}

	void SetFunction(std::function<void()> function)
	{
		workerFunction = function;
	}

	void Start()
	{
		isFinished = false;

		// Create a thread
		workerThread = std::thread([this]()
		{
			// Call the function
			workerFunction();

			// Flag its complete
			isFinished = true;
		});
	}

	bool TryJoin()
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

	std::function<void()> workerFunction;
	std::thread workerThread;

	bool isFinished;
};