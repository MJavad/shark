#include "Misc/stdafx.h"
#include "D3DManager.h"
#include "Log.h"
#include "WndProc.h"
#include "Engine.h"
#include "RenderTarget9.h"
#include "RenderTarget11.h"
#include "UI/D3DSprite9.h"
#include "UI/Components/IFocusable.h"

void D3DManager::Initialize() {
	sWndProc.OnMessageReceivedEvent.connect(boost::bind(
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
	m_sprite = boost::make_shared<UI::D3DSprite9>(pDevice);
	m_renderTarget = boost::make_shared<RenderTarget9>(pDevice);

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
		m_renderTarget = boost::make_shared<RenderTarget11>(
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
		m_renderTarget = boost::make_shared<RenderTarget11>(
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
		m_renderTarget = boost::make_shared<RenderTarget11>(
			m_swapChain, m_device11, m_deviceContext11);
	}
}

void D3DManager::PushInterface(boost::shared_ptr<ID3DInterface> pInterface) {
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
		(uMsg == WM_KEYDOWN || uMsg == WM_CHAR) &&
		wParam == VK_TAB) {
		auto pFocus = UI::Components::IFocusable::GetActiveFocus();
		if (pFocus != nullptr) {
			if (uMsg == WM_KEYDOWN)
				pFocus->OnTabPressed(pFocus);

			sWndProc.LastMessageHandled = true;
		}
	}
}

boost::shared_ptr<UI::D3DFont> D3DManager::GetFont(std::wstring fontName,
	uint32 height, uint32 width, uint32 weight, bool italic)
{
	size_t dwFontFace = std::hash<std::wstring>()(fontName);
	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_fonts.erase(itr);
			continue;
		}

		const auto pFont = (itr++)->lock();
		const auto &fontDesc = pFont->GetDescription();
		if (std::hash<std::wstring>()(fontDesc.faceName) == dwFontFace &&
			fontDesc.height == height && fontDesc.width == width &&
			fontDesc.weight == weight && fontDesc.italic == italic)
			return pFont;
	}

	UI::FontDescription fontDesc;
	fontDesc.height = height;
	fontDesc.width = width;
	fontDesc.weight = weight;
	fontDesc.italic = italic;
	fontDesc.mipLevels = 1;
	fontDesc.charSet = DEFAULT_CHARSET;
	fontDesc.outputPrecision = OUT_TT_PRECIS;
	fontDesc.quality = ANTIALIASED_QUALITY;
	fontDesc.pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	fontDesc.faceName = std::move(fontName);

	auto pFont = boost::make_shared<UI::D3DFont>(fontDesc);
	m_fonts.push_back(pFont);

	if (m_device9 != nullptr)
		pFont->SetDevice(m_device9);

	LOG_DEBUG(L"Font '%s' created!", fontDesc.faceName.c_str());
	return pFont;
}

boost::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromFile(
	const std::wstring &fileName, uint32 width, uint32 height)
{
	size_t fileNameHash = std::hash<std::wstring>()(fileName);
	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_textures.erase(itr);
			continue;
		}

		const auto pTexture = (itr++)->lock();
		const auto &texDesc = pTexture->GetDescription();
		if (texDesc.type == UI::TEXTURE_FROM_FILE &&
			std::hash<std::wstring>()(texDesc.filePath) == fileNameHash &&
			texDesc.width == width &&
			texDesc.height == height)
			return pTexture;
	}
	
	UI::TextureDescription texDesc;
	texDesc.type = UI::TEXTURE_FROM_FILE;
	texDesc.filePath = fileName;
	texDesc.resourceId = 0;
	texDesc.resourceModule = nullptr;
	texDesc.width = width;
	texDesc.height = height;
	texDesc.mipLevels = 1;
	texDesc.mipFilter = D3DX_DEFAULT;
	texDesc.format = D3DFMT_A8R8G8B8;
	texDesc.pool = D3DPOOL_DEFAULT;
	texDesc.filter = D3DX_DEFAULT;
	texDesc.colorKey = D3DXCOLOR(0, 0, 0, 0);
	texDesc.usage = 0;

	auto pTexture = boost::make_shared<UI::D3DTexture>(texDesc);
	m_textures.push_back(pTexture);

	if (m_device9 != nullptr)
		pTexture->SetDevice(m_device9);

	LOG_DEBUG(L"Texture from file '%s' created!", texDesc.filePath.c_str());
	return pTexture;
}

boost::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromResource(
	uint32 resourceId, HMODULE module, uint32 width, uint32 height)
{
	if (module == nullptr)
		module = sEngine.GetInstance();

	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_textures.erase(itr);
			continue;
		}

		const auto pTexture = (itr++)->lock();
		const auto &texDesc = pTexture->GetDescription();
		if (texDesc.type == UI::TEXTURE_FROM_RESOURCE &&
			texDesc.resourceId == resourceId &&
			texDesc.resourceModule == module &&
			texDesc.width == width &&
			texDesc.height == height)
			return pTexture;
	}

	UI::TextureDescription texDesc;
	texDesc.type = UI::TEXTURE_FROM_RESOURCE;
	texDesc.resourceId = resourceId;
	texDesc.resourceModule = module;
	texDesc.width = width;
	texDesc.height = height;
	texDesc.mipLevels = 1;
	texDesc.mipFilter = D3DX_DEFAULT;
	texDesc.format = D3DFMT_A8R8G8B8;
	texDesc.pool = D3DPOOL_DEFAULT;
	texDesc.filter = D3DX_DEFAULT;
	texDesc.colorKey = D3DXCOLOR(0, 0, 0, 0);
	texDesc.usage = 0;

	auto pTexture = boost::make_shared<UI::D3DTexture>(texDesc);
	m_textures.push_back(pTexture);

	if (m_device9 != nullptr)
		pTexture->SetDevice(m_device9);

	LOG_DEBUG(L"Texture from resource %u created!", texDesc.resourceId);
	return pTexture;
}

void D3DManager::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
	ID3DInterface::BindToLua(luaState);

	luabind::module(luaState.get()) [
		luabind::class_<D3DManager>("D3DManager")
			.scope [ luabind::def("GetInstance", &D3DManager::Instance) ]
			.def("PushInterface", &D3DManager::PushInterface)
			.def("PopInterface", &D3DManager::PopInterface)
	];
}
