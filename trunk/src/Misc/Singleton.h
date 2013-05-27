#pragma once

namespace Utils
{
	template <class T>
	class Singleton
	{
	protected:
		Singleton() {
			if (s_instance != nullptr) {
				std::string sMessage = "Instance of type '";
				sMessage += typeid(T).name();
				sMessage += "' is getting constructed the second (or more) time!";
				throw std::exception(sMessage.c_str());
			}

			s_instance = static_cast<T*>(this);
		}

		~Singleton() {
			s_instance = nullptr;
		}

	public:
		void Initialize() {}

		static T& Instance() {
			if (s_instance == nullptr) {
				std::string sMessage = "Tried to get instance for non initialized singleton '";
				sMessage += typeid(T).name();
				sMessage += "'.";
				throw std::exception(sMessage.c_str());
			}

			return *s_instance;
		}

		static bool Created() {
			return s_instance != nullptr;
		}

	private:
		static T *s_instance;

		Singleton(const Singleton<T>&);
		Singleton<T>& operator=(const Singleton<T>&);
	};
}

#define INIT_SINGLETON(T) T* Utils::Singleton<T>::s_instance = nullptr;
