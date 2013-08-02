/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	sWndProc.OnMessageReceivedEvent.connect(
		std::bind(&D3DManager::OnMessageReceived, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
}

void D3DManager::Shutdown() {
	Utils::ScopedLock g(m_frameLock);
	ReleaseDeviceResources();
	m_fonts.clear();
	m_textures.clear();
	m_interfaces.clear();
}

void D3DManager::ReleaseDeviceResources() {
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
	Utils::ScopedLock g(m_frameLock);
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
												   uint32 height,
												   uint32 weight,
												   bool italic)
{
	size_t fontNameHash = std::hash<std::wstring>() (fontName);
	for (auto itr = m_fonts.begin(), end = m_fonts.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_fonts.erase(itr);
			continue;
		}

		const auto font = (itr++)->lock();
		const auto &fontDesc = font->GetDescription();
		if (fontDesc.faceNameHash == fontNameHash &&
			fontDesc.height == height &&
			fontDesc.weight == weight &&
			fontDesc.italic == italic)
			return font;
	}

	UI::FontDescription fontDesc;
	fontDesc.height = height;
	fontDesc.width = 0;
	fontDesc.weight = weight;
	fontDesc.italic = italic;
	fontDesc.mipLevels = 1;
	fontDesc.charSet = DEFAULT_CHARSET;
	fontDesc.outputPrecision = OUT_TT_PRECIS;
	fontDesc.quality = ANTIALIASED_QUALITY;
	fontDesc.pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	fontDesc.faceName = std::move(fontName);
	fontDesc.faceNameHash = fontNameHash;

	auto font = boost::make_shared<UI::D3DFont>(fontDesc);
	m_fonts.push_back(font);

	if (m_device9 != nullptr)
		font->SetDevice(m_device9);

	LOG_DEBUG(L"Font '%s' created!", fontDesc.faceName.c_str());
	return font;
}

boost::shared_ptr<UI::D3DFont> D3DManager::GetFont_UTF8(const std::string &fontName,
														uint32 height,
														uint32 weight,
														bool italic)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return GetFont(conv.from_bytes(fontName), height, weight, italic);
}

boost::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromFile(std::wstring fileName,
																 uint32 width,
																 uint32 height)
{
	size_t fileNameHash = std::hash<std::wstring>() (fileName);
	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_textures.erase(itr);
			continue;
		}

		const auto texture = (itr++)->lock();
		const auto &texDesc = texture->GetDescription();
		if (texDesc.type == UI::TEXTURE_FROM_FILE &&
			texDesc.filePathHash == fileNameHash &&
			texDesc.width == width &&
			texDesc.height == height)
			return texture;
	}
	
	UI::TextureDescription texDesc;
	texDesc.type = UI::TEXTURE_FROM_FILE;
	texDesc.filePath = std::move(fileName);
	texDesc.filePathHash = fileNameHash;
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

	auto textureObject = boost::make_shared<UI::D3DTexture>(texDesc);
	m_textures.push_back(textureObject);

	if (m_device9 != nullptr)
		textureObject->SetDevice(m_device9);

	LOG_DEBUG(L"Texture from file '%s' created!", texDesc.filePath.c_str());
	return textureObject;
}

boost::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromFile_UTF8(const std::string &fileName,
																	  uint32 width,
																	  uint32 height)
{
	if (width == 0)
		width = D3DX_DEFAULT_NONPOW2;

	if (height == 0)
		height = D3DX_DEFAULT_NONPOW2;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return GetTextureFromFile(conv.from_bytes(fileName), width, height);
}

boost::shared_ptr<UI::D3DTexture> D3DManager::GetTextureFromResource(uint32 resourceId,
																	 HMODULE module,
																	 uint32 width,
																	 uint32 height)
{
	if (module == nullptr)
		module = sEngine.GetInstance();

	for (auto itr = m_textures.begin(), end = m_textures.end(); itr != end;) {
		if (itr->expired()) {
			itr = m_textures.erase(itr);
			continue;
		}

		const auto texture = (itr++)->lock();
		const auto &texDesc = texture->GetDescription();
		if (texDesc.type == UI::TEXTURE_FROM_RESOURCE &&
			texDesc.resourceId == resourceId &&
			texDesc.resourceModule == module &&
			texDesc.width == width &&
			texDesc.height == height)
			return texture;
	}

	UI::TextureDescription texDesc;
	texDesc.type = UI::TEXTURE_FROM_RESOURCE;
	texDesc.filePathHash = 0;
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

	auto texture = boost::make_shared<UI::D3DTexture>(texDesc);
	m_textures.push_back(texture);

	if (m_device9 != nullptr)
		texture->SetDevice(m_device9);

	LOG_DEBUG(L"Texture from resource %u created!", texDesc.resourceId);
	return texture;
}

void D3DManager::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
	ID3DInterface::BindToLua(luaState);

	class nothing { nothing() {} };

	luabind::module(luaState.get()) [
		luabind::class_<nothing>("FontWeight")
			.enum_("Constants") [
				luabind::value("Normal", FW_NORMAL),
				luabind::value("DontCare", FW_DONTCARE),
				luabind::value("SemiBold", FW_SEMIBOLD),
				luabind::value("Bold", FW_BOLD),
				luabind::value("Medium", FW_MEDIUM),
				luabind::value("ExtraBold", FW_EXTRABOLD),
				luabind::value("ExtraLight", FW_EXTRALIGHT),
				luabind::value("Heavy", FW_HEAVY),
				luabind::value("Light", FW_LIGHT),
				luabind::value("Thin", FW_THIN)
			],

		luabind::class_<UI::D3DFont, boost::shared_ptr<UI::D3DFont>>("D3DFont"),
		luabind::class_<UI::D3DTexture, boost::shared_ptr<UI::D3DTexture>>("D3DTexture"),

		luabind::class_<D3DManager>("D3DManager")
			.scope [ luabind::def("GetInstance", &D3DManager::Instance) ]
			.def("PushInterface", &D3DManager::PushInterface)
			.def("PopInterface", &D3DManager::PopInterface)
			.def("GetFont", &D3DManager::GetFont_UTF8)
			.def("GetTexture", &D3DManager::GetTextureFromFile_UTF8)
	];
}
