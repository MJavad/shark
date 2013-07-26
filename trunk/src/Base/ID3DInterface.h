#pragma once

class ID3DInterface abstract : public boost::enable_shared_from_this<ID3DInterface>
{
public:
	virtual ~ID3DInterface() {};
	virtual void OnRender(uint32 timePassed) = 0;
	virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

protected:
	ID3DInterface() {}

	template <typename T>
	boost::shared_ptr<T> get_this() {
		return boost::dynamic_pointer_cast<T>(shared_from_this());
	}

	template <typename T>
	boost::shared_ptr<T> get_this() const {
		return boost::dynamic_pointer_cast<T>(shared_from_this());
	}
};
