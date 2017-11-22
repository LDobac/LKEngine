#pragma once

#include "Macro.h"

template <typename T>
class TSingleton
{
private:
	static T* instance;
protected:
	TSingleton() { }
	virtual ~TSingleton() { }
public:
	static T * GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new T();
		}

		return instance;
	};

	static void Release()
	{
		if (instance)
		{
			SAFE_DELETE(instance);
		}
	};


};
template <typename T>  
T* TSingleton<T>::instance = nullptr;
