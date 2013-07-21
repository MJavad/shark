#pragma once
#include "UI/D3DFont.h"
#include "UI/D3DTexture.h"
#include "UI/ID3DSprite.h"
#include "ID3DInterface.h"
#include "IRenderTarget.h"

class D3DManager {
	SINGLETON_CLASS(D3DManager) :
		m_device9(nullptr),
		m_device11(nullptr),
		m_deviceContext11(nullptr),
		m_swapChain(nullptr),
		m_lastFrame(0),
		m_skipNextFrame(false) {}

public:
	void Initialize();
	void ReleaseDeviceResources();
	void SetDevice9(IDirect3DDevice9 *pDevice);
	void SetDevice11(ID3D11Device *pDevice);
	void SetDeviceContext11(ID3D11DeviceContext *pDeviceContext);
	void SetSwapChain(IDXGISwapChain *pSwapChain);

	IDirect3DDevice9* GetDevice9() const { return m_device9; }
	ID3D11Device* GetDevice11() const { return m_device11; }
	ID3D11DeviceContext* GetDeviceContext11() const { return m_deviceContext11; }
	IDXGISwapChain* GetSwapChain() const { return m_swapChain; }

	void PushInterface(boost::shared_ptr<ID3DInterface> pInterface);
	void PopInterface(const boost::shared_ptr<ID3DInterface> &pInterface) {
		m_interfaces.remove(pInterface);
	}

	void OnRender();
	void OnLostDevice();
	void OnResetDevice();
	void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void PrepareRendering() const;
	bool IsCreated() const { return m_renderTarget != nullptr; }

	boost::shared_ptr<IRenderTarget> GetRenderTarget() const { return m_renderTarget; }
	std::list<boost::shared_ptr<ID3DInterface>> GetInterfaces() const { return m_interfaces; }
	boost::shared_ptr<UI::ID3DSprite> GetSprite() const { return m_sprite; }

	boost::shared_ptr<UI::D3DFont> GetFont(std::wstring fontName, uint32 height,
		uint32 width = 0, uint32 weight = FW_NORMAL, bool italic = false);

	boost::shared_ptr<UI::D3DTexture> GetTextureFromFile(const std::wstring &fileName,
		uint32 width = D3DX_DEFAULT_NONPOW2, uint32 height = D3DX_DEFAULT_NONPOW2);

	boost::shared_ptr<UI::D3DTexture> GetTextureFromResource(uint32 resourceId, HMODULE module = nullptr,
		uint32 width = D3DX_DEFAULT_NONPOW2, uint32 height = D3DX_DEFAULT_NONPOW2);

	Utils::Event<void (uint32)> OnUpdateEvent;
	Utils::Event<void ()> OnDeviceLostEvent;
	Utils::Event<void ()> OnDeviceResetEvent;
	Utils::Event<void ()> OnDeviceChangedEvent;

protected:
	bool m_skipNextFrame;
	uint32 m_lastFrame;
	Utils::Mutex m_frameLock;

	IDirect3DDevice9 *m_device9;
	ID3D11Device *m_device11;
	ID3D11DeviceContext *m_deviceContext11;
	IDXGISwapChain *m_swapChain;
	boost::shared_ptr<UI::ID3DSprite> m_sprite;
	boost::shared_ptr<IRenderTarget> m_renderTarget;
	std::list<boost::shared_ptr<ID3DInterface>> m_interfaces;
	std::list<boost::weak_ptr<UI::D3DFont>> m_fonts;
	std::list<boost::weak_ptr<UI::D3DTexture>> m_textures;
};

#define sD3DMgr ::D3DManager::Instance()
