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

	inline void setFunction(function<T> function) { m_workerFunction = function; }

	inline void begin(bool isLooped = false)
	{
		m_completed = false;

		// create a thread
		m_workerThread = thread([this, isLooped]()
		{
			// Check if we want 
			if (isLooped)
			{
				while (!m_completed)
				{
					m_workerFunction();
				}
			}
			else
			{
				// Call the function
				m_workerFunction();

				// Flag its complete
				m_completed = true;
			}
		});
	}

	inline bool tryJoin()
	{
		if (m_completed && m_workerThread.joinable())
		{
			m_workerThread.join();
			return true;
		}

		return false;
	}

	inline void forceQuit()
	{
		m_completed = true;
		tryJoin();
	}

private:
	function<T> m_workerFunction;
	thread m_workerThread;
	bool m_completed;
};