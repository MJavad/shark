#pragma once

namespace Utils
{
	template<class T>
	class Singleton
	{
	protected:
		Singleton() {
			if (s_instance != nullptr) {
				std::string sMessage = "Instance of type '";
				sMessage += typeid(T).name();
				sMessage += "' is getting allocated the second (or more) time! See stacktrace!";
				throw std::exception(sMessage.c_str());
			}
		}

		~Singleton() {
			s_instance = nullptr;
		}

	public:
		void Initialize() {}

		static std::shared_ptr<T> Instance() {
			if (s_instance == nullptr)
				s_instance = std::make_shared<T>();

			return s_instance;
		}

		static bool Created() {
			return s_instance != nullptr;
		}

	private:
		static std::shared_ptr<T> s_instance;
	};
}

#define INIT_SINGLETON(T) std::shared_ptr<T> Utils::Singleton<T>::s_instance;
