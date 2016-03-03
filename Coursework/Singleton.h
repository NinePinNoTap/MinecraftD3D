#pragma once

#include <cassert>

template <typename T>
class Singleton
{
public:
	Singleton()
	{
		// Ensure we don't have an instance already
		assert(!Instance_);

		// Create an instance
		Instance_ = static_cast<T*>(this);
	}

	Singleton(Singleton const&)
	{
		// Ensure we don't have an instance already
		assert(!Instance_);

		// Create an instance
		Instance_ = static_cast<T*>(this);
	}

	~Singleton()
	{
		// Ensure we don't have an instance already
		assert(Instance_);

		// Delete instance
		Instance_ = 0;
	}

	static T* Instance()
	{
		// Returns the only instance of this object
		return Instance_;
	}

private:
	static T* Instance_;
};

template <typename T> T* Singleton<T>::Instance_ = NULL;


