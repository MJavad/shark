#include "Misc/stdafx.h"
#include "D3DManager.h"
#include "Log.h"
#include "WndProc.h"
#include "RenderTarget9.h"
#include "RenderTarget11.h"
#include "UI/D3DSprite9.h"
#include "UI/Components/IFocusable.h"

void D3DManager::Initialize() {
	using namespace std::placeholders;
	sWndProc.OnMessageReceivedEvent.connect(std::bind(
		&D3DManager::OnMessageReceived, this, _1, _2, _3));
}

void D3DManager::ReleaseDeviceResources() {
	Utils::LockGuard g(m_frameLock);
	m_sprite.reset();
	m_renderTarget.reset();
	m_device9 = nullptr;
	m_device11 = nullptr;
	m_deviceContext11 = nullptr;
	m_swapChain = nullptr;
	m_skipNextFrame = true;

	// Release all font resources...
	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired())
			itr = m_fonts.erase(itr);
		else
			(itr++)->lock()->Release();
	}

	// Release all texture resources...
	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired())
			itr = m_textures.erase(itr);
		else
			(itr++)->lock()->Release();
	}

	// Tell our controls that our device has changed -> flushes resources
	OnDeviceChangedEvent();
}

void D3DManager::SetDevice9(IDirect3DDevice9 *pDevice) {
	Utils::LockGuard g(m_frameLock);
	if (m_skipNextFrame) {
		m_skipNextFrame = false;
		return;
	}

	m_device9 = pDevice;
	m_device11 = nullptr;
	m_deviceContext11 = nullptr;
	m_swapChain = nullptr;
	m_sprite = std::make_shared<UI::D3DSprite9>(pDevice);
	m_renderTarget = std::make_shared<RenderTarget9>(pDevice);

	D3DDEVICE_CREATION_PARAMETERS creationParams = {0};
	if (pDevice->GetCreationParameters(&creationParams) == D3D_OK)
		sWndProc.Attach(creationParams.hFocusWindow);

	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired())
			itr = m_fonts.erase(itr);
		else
			(itr++)->lock()->SetDevice(pDevice);
	}

	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired())
			itr = m_textures.erase(itr);
		else
			(itr++)->lock()->SetDevice(pDevice);
	}

	// call events...
	OnDeviceChangedEvent();
}

void D3DManager::SetDevice11(ID3D11Device *pDevice) {
	Utils::LockGuard g(m_frameLock);
	m_device9 = nullptr;
	m_device11 = pDevice;
	m_renderTarget.reset();

	if (m_device11 != nullptr &&
		m_deviceContext11 != nullptr &&
		m_swapChain != nullptr) {
		m_renderTarget = std::make_shared<RenderTarget11>(
			m_swapChain, m_device11, m_deviceContext11);
	}
}

void D3DManager::SetDeviceContext11(ID3D11DeviceContext *pDeviceContext) {
	Utils::LockGuard g(m_frameLock);
	m_deviceContext11 = pDeviceContext;
	m_renderTarget.reset();

	if (m_device11 != nullptr &&
		m_deviceContext11 != nullptr &&
		m_swapChain != nullptr) {
		m_renderTarget = std::make_shared<RenderTarget11>(
			m_swapChain, m_device11, m_deviceContext11);
	}
}

void D3DManager::SetSwapChain(IDXGISwapChain *pSwapChain) {
	Utils::LockGuard g(m_frameLock);
	m_swapChain = pSwapChain;
	m_renderTarget.reset();

	if (m_device11 != nullptr &&
		m_deviceContext11 != nullptr &&
		m_swapChain != nullptr) {
		m_renderTarget = std::make_shared<RenderTarget11>(
			m_swapChain, m_device11, m_deviceContext11);
	}
}

void D3DManager::PushInterface(std::shared_ptr<ID3DInterface> pInterface) {
	m_interfaces.remove(pInterface);
	m_interfaces.push_front(std::move(pInterface));
}

void D3DManager::OnRender() {
	Utils::LockGuard g(m_frameLock);
	if (m_skipNextFrame) {
		m_skipNextFrame = false;
		return;
	}

	uint32 time_now = timeGetTime();
	uint32 time_diff = time_now - m_lastFrame;
	m_lastFrame = time_now;

	// update current frame...
	OnUpdateEvent(time_diff);

	m_renderTarget->BeginUI();
	for (auto itr = m_interfaces.rbegin(), end = m_interfaces.rend(); itr != end; ++itr)
		(*itr)->OnRender(time_diff);
	m_renderTarget->EndUI();
}

void D3DManager::OnLostDevice() {
	Utils::LockGuard g(m_frameLock);
	m_skipNextFrame = true;

	if (m_sprite != nullptr)
		m_sprite->OnLostDevice();

	if (m_renderTarget != nullptr)
		m_renderTarget->OnLostDevice();

	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired())
			itr = m_fonts.erase(itr);
		else
			(itr++)->lock()->OnLostDevice();
	}

	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired())
			itr = m_textures.erase(itr);
		else
			(itr++)->lock()->OnLostDevice();
	}

	OnDeviceLostEvent();
}

void D3DManager::OnResetDevice() {
	Utils::LockGuard g(m_frameLock);
	m_skipNextFrame = true;

	if (m_sprite != nullptr)
		m_sprite->OnResetDevice();

	if (m_renderTarget != nullptr)
		m_renderTarget->OnResetDevice();

	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired())
			itr = m_fonts.erase(itr);
		else
			(itr++)->lock()->OnResetDevice();
	}

	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired())
			itr = m_textures.erase(itr);
		else
			(itr++)->lock()->OnResetDevice();
	}

	OnDeviceResetEvent();
}

void D3DManager::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Utils::LockGuard g(m_frameLock);
	const auto lstInterfaces = m_interfaces;
	for( auto itr = lstInterfaces.begin(), end = lstInterfaces.end(); itr != end; ++itr )
		(*itr)->OnMessageReceived(uMsg, wParam, lParam);

	if (!sWndProc.LastMessageHandled &&
		uMsg == WM_KEYDOWN && wParam == VK_TAB) {
		auto pFocus = UI::Components::IFocusable::GetActiveFocus();

		if (pFocus != nullptr) {
			pFocus->OnTabPressed(pFocus);
			sWndProc.LastMessageHandled = true;
		}
	}
}

std::shared_ptr<UI::D3DFont> D3DManager::GetFont(std::wstring swFontName,
	uint32 uHeight, uint32 uWidth, uint32 uWeight, bool bItalic)
{
	size_t dwFontFace = std::hash<std::wstring>()(swFontName);
	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_fonts.erase(itr);
			continue;
		}

		const auto pFont = (itr++)->lock();
		const auto &fontDesc = pFont->GetDescription();
		if (std::hash<std::wstring>()(fontDesc.FaceName) == dwFontFace &&
			fontDesc.Height == uHeight && fontDesc.Width == uWidth &&
			fontDesc.Weight == uWeight && fontDesc.Italic == bItalic)
			return pFont;
	}

	UI::SFontDesc fontDesc = {0};
	fontDesc.Height = uHeight;
	fontDesc.Width = uWidth;
	fontDesc.Weight = uWeight;
	fontDesc.Italic = bItalic;
	fontDesc.MipLevels = 1;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_TT_PRECIS;
	fontDesc.Quality = ANTIALIASED_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	fontDesc.FaceName = std::move(swFontName);

	auto pFont = std::make_shared<UI::D3DFont>(fontDesc);
	m_fonts.push_back(pFont);

	if (m_device9 != nullptr)
		pFont->SetDevice(m_device9);

	LOG_DEBUG(L"Font '%s' created!", fontDesc.FaceName.c_str());
	return pFont;
}

std::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromFile(
	std::wstring swFileName, uint32 uWidth, uint32 uHeight)
{
	size_t dwFileName = std::hash<std::wstring>()(swFileName);
	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_textures.erase(itr);
			continue;
		}

		const auto pTexture = (itr++)->lock();
		const auto &texDesc = pTexture->GetDescription();
		if (texDesc.TextureType == UI::TEXTURE_FROM_FILE &&
			std::hash<std::wstring>()(texDesc.FilePathOrResource) == dwFileName &&
			texDesc.Width == uWidth &&
			texDesc.Height == uHeight)
			return pTexture;
	}
	
	UI::STextureDesc texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.TextureType = UI::TEXTURE_FROM_FILE;
	texDesc.FilePathOrResource = std::move(swFileName);
	texDesc.Width = uWidth;
	texDesc.Height = uHeight;
	texDesc.MipLevels = 1;
	texDesc.Format = D3DFMT_A8R8G8B8;
	texDesc.Pool = D3DPOOL_DEFAULT;
	texDesc.Filter = D3DX_DEFAULT;
	texDesc.MipFilter = D3DX_DEFAULT;

	auto pTexture = std::make_shared<UI::D3DTexture>(texDesc);
	m_textures.push_back(pTexture);

	if (m_device9 != nullptr)
		pTexture->SetDevice(m_device9);

	LOG_DEBUG(L"Texture from file '%s' created!", texDesc.FilePathOrResource.c_str());
	return pTexture;
}

std::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromResource(
	std::wstring swResourceName, HMODULE hModule, uint32 uWidth, uint32 uHeight)
{
	size_t dwResourceName = std::hash<std::wstring>()(swResourceName);
	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_textures.erase(itr);
			continue;
		}

		const auto pTexture = (itr++)->lock();
		const auto &texDesc = pTexture->GetDescription();
		if (texDesc.TextureType == UI::TEXTURE_FROM_RESOURCE &&
			std::hash<std::wstring>()(texDesc.FilePathOrResource) == dwResourceName &&
			texDesc.ResourceModule == hModule &&
			texDesc.Width == uWidth &&
			texDesc.Height == uHeight)
			return pTexture;
	}

	UI::STextureDesc texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.TextureType = UI::TEXTURE_FROM_RESOURCE;
	texDesc.FilePathOrResource = std::move(swResourceName);
	texDesc.ResourceModule = hModule;
	texDesc.Width = uWidth;
	texDesc.Height = uHeight;
	texDesc.MipLevels = 1;
	texDesc.Format = D3DFMT_A8R8G8B8;
	texDesc.Pool = D3DPOOL_DEFAULT;
	texDesc.Filter = D3DX_DEFAULT;
	texDesc.MipFilter = D3DX_DEFAULT;

	auto pTexture = std::make_shared<UI::D3DTexture>(texDesc);
	m_textures.push_back(pTexture);

	if (m_device9 != nullptr)
		pTexture->SetDevice(m_device9);

	LOG_DEBUG(L"Texture from resource '%s' created!", texDesc.FilePathOrResource.c_str());
	return pTexture;
}
