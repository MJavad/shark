#pragma once
#include "UI/D3DFont.h"
#include "UI/D3DTexture.h"
#include "UI/ID3DSprite.h"
#include "ID3DInterface.h"
#include "IRenderTarget.h"

class D3DManager : public Utils::Singleton
{
	SINGLETON_OBJECT
	D3DManager() : m_device9(nullptr),
				   m_device11(nullptr),
				   m_deviceContext11(nullptr),
				   m_swapChain(nullptr),
				   m_lastFrame(0) {}

public:
	void Initialize();
	void SetDevice9(IDirect3DDevice9 *pDevice);
	void SetDevice11(ID3D11Device *pDevice);
	void SetDeviceContext11(ID3D11DeviceContext *pDeviceContext);
	void SetSwapChain(IDXGISwapChain *pSwapChain);

	IDirect3DDevice9* GetDevice9() const { return m_device9; }
	ID3D11Device* GetDevice11() const { return m_device11; }
	ID3D11DeviceContext* GetDeviceContext11() const { return m_deviceContext11; }
	IDXGISwapChain* GetSwapChain() const { return m_swapChain; }

	void PushInterface(std::shared_ptr<ID3DInterface> pInterface);
	void PopInterface(const std::shared_ptr<ID3DInterface> &pInterface) {
		m_interfaces.remove(pInterface);
	}

	void OnRender();
	void OnLostDevice();
	void OnResetDevice();
	void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void PrepareRendering() const;
	bool IsCreated() const { return m_renderTarget != nullptr; }

	std::shared_ptr<IRenderTarget> GetRenderTarget() const { return m_renderTarget; }
	std::list<std::shared_ptr<ID3DInterface>> GetInterfaces() const { return m_interfaces; }
	std::shared_ptr<UI::ID3DSprite> GetSprite() const { return m_sprite; }

	std::shared_ptr<UI::D3DFont> GetFont(std::wstring swFontName, uint32 uHeight,
		uint32 uWidth = 0, uint32 uWeight = FW_NORMAL, bool bItalic = false);

	std::shared_ptr<UI::D3DTexture> GetTextureFromFile(std::wstring swFileName,
		uint32 uWidth = D3DX_DEFAULT_NONPOW2, uint32 uHeight = D3DX_DEFAULT_NONPOW2);

	std::shared_ptr<UI::D3DTexture> GetTextureFromResource(std::wstring swResourceName,
		HMODULE hModule, uint32 uWidth = D3DX_DEFAULT_NONPOW2, uint32 uHeight = D3DX_DEFAULT_NONPOW2);

	Utils::Event<void (uint32)> OnUpdateEvent;
	Utils::Event<void ()> OnDeviceLostEvent;
	Utils::Event<void ()> OnDeviceResetEvent;
	Utils::Event<void ()> OnDeviceChangedEvent;

protected:
	uint32 m_lastFrame;
	Utils::Mutex m_frameLock;

	IDirect3DDevice9 *m_device9;
	ID3D11Device *m_device11;
	ID3D11DeviceContext *m_deviceContext11;
	IDXGISwapChain *m_swapChain;
	std::shared_ptr<UI::ID3DSprite> m_sprite;
	std::shared_ptr<IRenderTarget> m_renderTarget;
	std::list<std::shared_ptr<ID3DInterface>> m_interfaces;
	std::list<std::weak_ptr<UI::D3DFont>> m_fonts;
	std::list<std::weak_ptr<UI::D3DTexture>> m_textures;
};

#define sD3DMgr GET_INSTANCE(D3DManager)
