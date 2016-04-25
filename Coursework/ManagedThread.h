#pragma once

#include <functional>
#include <thread>

using namespace std;

class ManagedThread
{
public:
	ManagedThread();
	~ManagedThread();

	// Functionality
	void Start(bool isLooped = false);
	bool TryJoin();
	void ForceClose();

	// Setters
	void SetFunction(function<void()> function);

private:
	function<void()> workerFunction;
	thread workerThread;

	bool isFinished;
};