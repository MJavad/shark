#pragma once

namespace Utils
{
	template <class T>
	class Singleton
	{
	public:
		static T& Instance() {
			static T sInstance;
			return sInstance;
		}

	protected:
		Singleton() {}

	private:
		Singleton(const Singleton<T>&);
		Singleton<T>& operator=(const Singleton<T>&);
	};
}

#define SINGLETON_OBJ(T) friend class ::Utils::Singleton<T>
