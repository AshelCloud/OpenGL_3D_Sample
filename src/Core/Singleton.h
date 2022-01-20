#pragma once

#include <memory>
#include <mutex>

template <class T>
class Singleton
{
protected:
	Singleton() = default;

public:
	static T& GetInstance()
	{
		std::call_once(onceflag, []
		{
				instance.reset(new T());
		});

		return *instance.get();
	}

private:
	static std::unique_ptr<T> instance;
	static std::once_flag onceflag;
};

template<class T>
std::unique_ptr<T> Singleton<T>::instance = nullptr;

template <class T>
std::once_flag Singleton<T>::onceflag;