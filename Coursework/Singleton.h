#pragma once

#include <cassert>

template <typename T>
class Singleton
{
public:
	Singleton()
	{
		// Ensure we don't have an instance already
		assert(!m_instance);

		// create an instance
		m_instance = static_cast<T*>(this);
	}

	Singleton(Singleton const&)
	{
		// Ensure we don't have an instance already
		assert(!m_instance);

		// create an instance
		m_instance = static_cast<T*>(this);
	}

	~Singleton()
	{
		// Ensure we don't have an instance already
		assert(m_instance);

		// Delete instance
		m_instance = 0;
	}

	static T* getInstance()
	{
		assert(m_instance);

		// Returns the only instance of this object
		return m_instance;
	}

private:
	static T* m_instance;
};

template <typename T> T* Singleton<T>::m_instance = NULL;


