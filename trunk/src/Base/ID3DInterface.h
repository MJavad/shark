#pragma once

class ID3DInterface abstract : public std::enable_shared_from_this<ID3DInterface>
{
public:
	virtual ~ID3DInterface() {};
	virtual void OnRender(uint32 timePassed) = 0;
	virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:
	template <typename T>
	std::shared_ptr<T> get_this() {
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	template <typename T>
	std::shared_ptr<T> get_this() const {
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}
};
