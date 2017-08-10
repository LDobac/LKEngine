#pragma once

#include <memory>
#include <functional>
#include <cassert>

namespace LKEngine
{
	namespace Core
	{
		//std::shared_ptr wrapper class
		template <typename Object_T>
		class ManagedObject
		{
		private:
			std::shared_ptr<Object_T> object;
		public:
			explicit ManagedObject(Object_T* newObject, bool isArray = false)
			{
				assert(nullptr != newObject);
				if (isArray)
				{
					object = std::shared_ptr<Object_T>(newObject);
				}
				else
				{
					object = std::shared_ptr<Object_T>(newObject, [](Object_T* arr) {delete[] arr; });
				}
			}

			template<typename Deleter_T>
			explicit ManagedObject(Object_T* newObject, Deleter_T deleter)
			{
				assert(nullptr != newObject);
				object = std::shared_ptr<Object_T>(newObject, deleter);
			}
			
			Object_T& operator*() const 
			{
				return *object.get();
			}
			Object_T* operator->() const
			{
				return object.get();
			}
			void operator=(std::nullptr_t null)
			{
				object = null;
			}

			void Swap(ManagedObject<Object_T> other)
			{
				object.swap(other->GetRawShared());
			}
			void Release()
			{
				object.reset();
			}

			std::shared_ptr<Object_T> GetRawShared()
			{
				return object;
			}

			~ManagedObject()
			{
				Release();
			}
		};
	}
}