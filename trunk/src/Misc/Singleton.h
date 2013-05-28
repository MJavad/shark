#pragma once

namespace Utils
{
	class Singleton
	{
	public:
		template <class T>
		static typename std::enable_if<
			std::is_base_of<Singleton, T>::value, T&>::type
			Instance() {
			static T sInstance;
			return sInstance;
		}

	protected:
		Singleton() {}

	private:
		Singleton(const Singleton&);
		Singleton& operator=(const Singleton&);
	};
}

#define SINGLETON_OBJECT friend class ::Utils::Singleton;
#define GET_INSTANCE(T) ::Utils::Singleton::Instance<::T>()

#define SINGLETON_CLASS(T) \
public:		static T& Instance() { \
				static T sInstance; \
				return sInstance; \
			} \
private:	T(const T&); \
			T& operator=(const T&); \
			T()
