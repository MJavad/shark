#include "Misc/stdafx.h"
#include "RenderTarget9.h"
#include "UI/D3DTextureObject9.h"
#include "UI/D3DSurface9.h"

struct SVertex {
    float x, y, z, r;
    uint32 color;
    static const uint32 FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
};

struct SVertexTex {
	float x, y, z, u, v;
	float4 color;
};

struct SVertexTexFVF {
	float x, y, z, r;
	uint32 color;
	float u, v;
	static const uint32 FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};

RenderTarget9::RenderTarget9(IDirect3DDevice9 *pDevice) : m_device9(pDevice) {
	D3DVERTEXELEMENT9 vertexElem[] = {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	HRESULT hResult = m_device9->CreateVertexDeclaration(vertexElem, &m_vertexTexDecl);
	m_stateBlock = _createStateBlock(D3DSBT_ALL);
	m_defaultStateBlock = _createDefaultStateBlock();

	CComPtr<ID3DXBuffer> pVertexMain = nullptr;
	hResult = D3DXCompileShaderFromFileW(L"E:\\shark\\RoundedRectangle.hlsl", nullptr, nullptr, "VertexMain", "vs_3_0", 0, &pVertexMain, nullptr, nullptr);
	assert(hResult == D3D_OK);
	hResult = m_device9->CreateVertexShader(reinterpret_cast<DWORD*>(pVertexMain->GetBufferPointer()), &m_roundRectVertex);
	assert(hResult == D3D_OK);

	CComPtr<ID3DXBuffer> pPixelMainDraw = nullptr;
	hResult = D3DXCompileShaderFromFileW(L"E:\\shark\\RoundedRectangle.hlsl", nullptr, nullptr, "PixelMainDraw", "ps_3_0", 0, &pPixelMainDraw, nullptr, nullptr);
	assert(hResult == D3D_OK);
	hResult = m_device9->CreatePixelShader(reinterpret_cast<DWORD*>(pPixelMainDraw->GetBufferPointer()), &m_roundRectDrawPixel);
	assert(hResult == D3D_OK);

	CComPtr<ID3DXBuffer> pPixelMainFill = nullptr;
	hResult = D3DXCompileShaderFromFileW(L"E:\\shark\\RoundedRectangle.hlsl", nullptr, nullptr, "PixelMainFill", "ps_3_0", 0, &pPixelMainFill, nullptr, nullptr);
	assert(hResult == D3D_OK);
	hResult = m_device9->CreatePixelShader(reinterpret_cast<DWORD*>(pPixelMainFill->GetBufferPointer()), &m_roundRectFillPixel);
	assert(hResult == D3D_OK);

	CComPtr<ID3DXBuffer> pVertexMainBlur = nullptr;
	hResult = D3DXCompileShaderFromFileW(L"E:\\shark\\Blur.hlsl", nullptr, nullptr, "VertexMain", "vs_3_0", 0, &pVertexMainBlur, nullptr, nullptr);
	assert(hResult == D3D_OK);
	hResult = m_device9->CreateVertexShader(reinterpret_cast<DWORD*>(pVertexMainBlur->GetBufferPointer()), &m_blurVertex);
	assert(hResult == D3D_OK);

	CComPtr<ID3DXBuffer> pPixelMainBlur = nullptr;
	hResult = D3DXCompileShaderFromFileW(L"E:\\shark\\Blur.hlsl", nullptr, nullptr, "PixelMain", "ps_3_0", 0, &pPixelMainBlur, nullptr, nullptr);
	assert(hResult == D3D_OK);
	hResult = m_device9->CreatePixelShader(reinterpret_cast<DWORD*>(pPixelMainBlur->GetBufferPointer()), &m_blurPixel);
	assert(hResult == D3D_OK);
}

void RenderTarget9::BeginUI() {
	m_oldScissorEnable = FALSE;
	m_device9->GetScissorRect(&m_oldScissorRect);
	m_device9->GetRenderState(D3DRS_SCISSORTESTENABLE, &m_oldScissorEnable);

	m_stateBlock->Capture();
	m_defaultStateBlock->Apply();

	RECT screenRect = {0};
	if (GetSurfaceRect(&screenRect)) {
		D3DVIEWPORT9 viewport = {0};
		viewport.Width = screenRect.right - screenRect.left;
		viewport.Height = screenRect.bottom - screenRect.top;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 0.94f;
		m_device9->SetViewport(&viewport);
	}
}

void RenderTarget9::EndUI() {
	m_stateBlock->Apply();
	m_device9->SetScissorRect(&m_oldScissorRect);
	m_device9->SetRenderState(D3DRS_SCISSORTESTENABLE, m_oldScissorEnable);
}

void RenderTarget9::OnLostDevice() {
	m_stateBlock.Release();
	m_defaultStateBlock.Release();
}

void RenderTarget9::OnResetDevice() {
	m_stateBlock = _createStateBlock(D3DSBT_ALL);
	m_defaultStateBlock = _createDefaultStateBlock();
}

bool RenderTarget9::GetSurfaceRect(RECT *pRect) const {
	if (pRect != nullptr) {
		ZeroMemory(pRect, sizeof(RECT));
		D3DSURFACE_DESC surfaceDesc;
		IDirect3DSurface9 *pRenderTarget = nullptr;
		if (m_device9->GetRenderTarget(0, &pRenderTarget) == D3D_OK) {
			HRESULT hResult = pRenderTarget->GetDesc(&surfaceDesc);
			pRenderTarget->Release();

			if (hResult == D3D_OK) {
				pRect->right = surfaceDesc.Width;
				pRect->bottom = surfaceDesc.Height;
				return true;
			}
		}
	}

	return false;
}

bool RenderTarget9::GetClippingArea(RECT *pRect) const {
	DWORD dwScissorEnabled = FALSE;
	m_device9->GetRenderState(D3DRS_SCISSORTESTENABLE, &dwScissorEnabled);
	if (dwScissorEnabled == FALSE)
		return false;

	return m_device9->GetScissorRect(pRect) == D3D_OK;
}

void RenderTarget9::SetClippingArea(const RECT *pRect) const {
	m_device9->SetRenderState(D3DRS_SCISSORTESTENABLE, pRect != nullptr);

	if (pRect != nullptr)
		m_device9->SetScissorRect(pRect);
}

std::shared_ptr<UI::D3DTexture> RenderTarget9::CreateRenderTargetTexture(uint32 uWidth, uint32 uHeight) const {
	CComPtr<IDirect3DTexture9> pTexture = nullptr;
	HRESULT hResult = D3DXCreateTexture(m_device9, uWidth, uHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture);
	if (hResult != D3D_OK)
		return nullptr;

	auto pTextureObject = std::make_shared<UI::D3DTextureObject9>(pTexture);
	return std::make_shared<UI::D3DTexture>(pTextureObject);
}

std::shared_ptr<UI::ID3DSurface> RenderTarget9::CreateRenderTargetSurface(uint32 uWidth, uint32 uHeight) const {
	CComPtr<IDirect3DSurface9> pBackBuffer = nullptr;
	HRESULT hResult = m_device9->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	if (hResult != D3D_OK)
		return nullptr;

	D3DSURFACE_DESC backBufferDesc;
	hResult = pBackBuffer->GetDesc(&backBufferDesc);
	if (hResult != D3D_OK)
		return nullptr;

	CComPtr<IDirect3DSurface9> pSurface = nullptr;
	hResult = m_device9->CreateRenderTarget(uWidth, uHeight, backBufferDesc.Format,
		backBufferDesc.MultiSampleType, backBufferDesc.MultiSampleQuality, FALSE, &pSurface, nullptr);
	if (hResult != D3D_OK)
		return nullptr;

	return std::make_shared<UI::D3DSurface9>(pSurface);
}

void RenderTarget9::SetRenderTargetSurface(const std::shared_ptr<const UI::ID3DSurface> &pSurface, uint32 uIndex, bool bClear) {
	CComPtr<IDirect3DSurface9> pOldSurface = nullptr;
	auto pSurfaceObject = std::dynamic_pointer_cast<const UI::D3DSurface9>(pSurface);

	if (pSurfaceObject != nullptr) {
		auto pSurface9 = pSurfaceObject->GetSurface9();
		if (pSurface9 != nullptr) {
			m_device9->SetRenderTarget(uIndex, pSurface9);
			
			if (bClear)
				m_device9->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 0.0f, 0);
		}
	}
}

std::shared_ptr<UI::ID3DSurface> RenderTarget9::GetRenderTargetSurface(uint32 uIndex) const {
	CComPtr<IDirect3DSurface9> pSurface = nullptr;
	HRESULT hResult = m_device9->GetRenderTarget(uIndex, &pSurface);
	if (hResult != D3D_OK)
		return nullptr;

	return std::make_shared<UI::D3DSurface9>(pSurface);
}

// Normal
void RenderTarget9::DrawRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
								  uint32 dwColor, float fStroke) const {
	float fWidth = GetDimensionWidth(dimensions);
	float fHeight = GetDimensionHeight(dimensions);

	SVertex vertices_top[] = {
		{ vPosition.x, vPosition.y, 0, 1, dwColor },
		{ vPosition.x + fWidth - fStroke, vPosition.y, 0, 1, dwColor },
		{ vPosition.x + fWidth - fStroke, vPosition.y + fStroke, 0, 1, dwColor },
		{ vPosition.x, vPosition.y + fStroke, 0, 1, dwColor }
	};

	SVertex vertices_bottom[] = {
		{ vPosition.x + fStroke, vPosition.y + fHeight - fStroke, 0, 1, dwColor },
		{ vPosition.x + fWidth, vPosition.y + fHeight - fStroke, 0, 1, dwColor },
		{ vPosition.x + fWidth, vPosition.y + fHeight, 0, 1, dwColor },
		{ vPosition.x + fStroke, vPosition.y + fHeight, 0, 1, dwColor }
	};

	SVertex vertices_left[] = {
		{ vPosition.x, vPosition.y + fStroke, 0, 1, dwColor },
		{ vPosition.x + fStroke, vPosition.y + fStroke, 0, 1, dwColor },
		{ vPosition.x + fStroke, vPosition.y + fHeight, 0, 1, dwColor },
		{ vPosition.x, vPosition.y + fHeight, 0, 1, dwColor }
	};

	SVertex vertices_right[] = {
		{ vPosition.x + fWidth - fStroke, vPosition.y, 0, 1, dwColor },
		{ vPosition.x + fWidth, vPosition.y, 0, 1, dwColor },
		{ vPosition.x + fWidth, vPosition.y + fHeight - fStroke, 0, 1, dwColor },
		{ vPosition.x + fWidth - fStroke, vPosition.y + fHeight - fStroke, 0, 1, dwColor }
	};

	m_device9->SetFVF(SVertex::FVF);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices_top, sizeof(SVertex));
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices_bottom, sizeof(SVertex));
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices_left, sizeof(SVertex));
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices_right, sizeof(SVertex));
}

void RenderTarget9::FillRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor) const {
	SVertex vertices[] = {
		{ vPosition.x + dimensions[0].x, vPosition.y + dimensions[0].y, 0, 1, dwColor },
		{ vPosition.x + dimensions[1].x, vPosition.y + dimensions[1].y, 0, 1, dwColor },
		{ vPosition.x + dimensions[2].x, vPosition.y + dimensions[2].y, 0, 1, dwColor },
		{ vPosition.x + dimensions[3].x, vPosition.y + dimensions[3].y, 0, 1, dwColor }
	};

	m_device9->SetFVF(SVertex::FVF);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertex));
}

void RenderTarget9::DrawRoundedRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
										 const float4 &fHorizRadius, const float4 &fVertRadius, uint32 dwColor, float fStroke) const {
	Utils::Color color(dwColor);
	float4 fColor = {
		color.R / 255.0f, color.G / 255.0f,
		color.B / 255.0f, color.A / 255.0f
	};

	SVertexTex vertices[] = {
		{ vPosition.x + dimensions[0].x - 2, vPosition.y + dimensions[0].y - 2, 0, 0, 0, fColor },
		{ vPosition.x + dimensions[1].x + 2, vPosition.y + dimensions[1].y - 2, 0, 1, 0, fColor },
		{ vPosition.x + dimensions[2].x + 2, vPosition.y + dimensions[2].y + 2, 0, 1, 1, fColor },
		{ vPosition.x + dimensions[3].x - 1, vPosition.y + dimensions[3].y + 2, 0, 0, 1, fColor }
	};

	m_device9->SetVertexShader(m_roundRectVertex);
	m_device9->SetPixelShader(m_roundRectDrawPixel);

	D3DVIEWPORT9 viewPort = {0};
	m_device9->GetViewport(&viewPort);
	float fScreenWidth = (float) viewPort.Width;
	float fScreenHeight = (float) viewPort.Height;
	
	D3DXMATRIX orthoProj;
	D3DXMatrixOrthoLH(&orthoProj, fScreenWidth, fScreenHeight, viewPort.MinZ, viewPort.MaxZ);
	m_device9->SetVertexShaderConstantF(0, orthoProj, 4);
	
	float fWidth = GetDimensionWidth(dimensions) + 4;
	float fHeight = GetDimensionHeight(dimensions) + 4;
	float params1[] = { fStroke, fStroke / fWidth, 0, 0 };
	m_device9->SetPixelShaderConstantF(4, params1, 1);

	float radius[] = { fHorizRadius._1, fHorizRadius._2, fHorizRadius._3, fHorizRadius._4,
					   fVertRadius._1, fVertRadius._2, fVertRadius._3, fVertRadius._4 };
	m_device9->SetPixelShaderConstantF(6, radius, 2);

	float params2[] = { fWidth, fHeight, fScreenWidth, fScreenHeight };
	m_device9->SetPixelShaderConstantF(5, params2, 1);
	m_device9->SetVertexShaderConstantF(5, params2, 1);

	m_device9->SetVertexDeclaration(m_vertexTexDecl);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertexTex));

	m_device9->SetVertexShader(nullptr);
	m_device9->SetPixelShader(nullptr);
}

void RenderTarget9::FillRoundedRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
										 const float4 &fHorizRadius, const float4 &fVertRadius, uint32 dwColor) const {
	Utils::Color color(dwColor);
	float4 fColor = {
		color.R / 255.0f, color.G / 255.0f,
		color.B / 255.0f, color.A / 255.0f
	};

	SVertexTex vertices[] = {
		{ vPosition.x + dimensions[0].x - 2, vPosition.y + dimensions[0].y - 2, 0, 0, 0, fColor },
		{ vPosition.x + dimensions[1].x + 2, vPosition.y + dimensions[1].y - 2, 0, 1, 0, fColor },
		{ vPosition.x + dimensions[2].x + 2, vPosition.y + dimensions[2].y + 2, 0, 1, 1, fColor },
		{ vPosition.x + dimensions[3].x - 2, vPosition.y + dimensions[3].y + 2, 0, 0, 1, fColor }
	};

	m_device9->SetVertexShader(m_roundRectVertex);
	m_device9->SetPixelShader(m_roundRectFillPixel);

	D3DVIEWPORT9 viewPort = {0};
	m_device9->GetViewport(&viewPort);
	float fScreenWidth = (float) viewPort.Width;
	float fScreenHeight = (float) viewPort.Height;
	
	D3DXMATRIX orthoProj;
	D3DXMatrixOrthoLH(&orthoProj, fScreenWidth, fScreenHeight, viewPort.MinZ, viewPort.MaxZ);
	m_device9->SetVertexShaderConstantF(0, orthoProj, 4);
	
	float fWidth = GetDimensionWidth(dimensions) + 4;
	float fHeight = GetDimensionHeight(dimensions) + 4;
	float params1[] = { 1.0f, 1.0f / fWidth, 0, 0 };
	m_device9->SetPixelShaderConstantF(4, params1, 1);

	float radius[] = { fHorizRadius._1, fHorizRadius._2, fHorizRadius._3, fHorizRadius._4,
					   fVertRadius._1, fVertRadius._2, fVertRadius._3, fVertRadius._4 };
	m_device9->SetPixelShaderConstantF(6, radius, 2);

	float params2[] = { fWidth, fHeight, fScreenWidth, fScreenHeight };
	m_device9->SetPixelShaderConstantF(5, params2, 1);
	m_device9->SetVertexShaderConstantF(5, params2, 1);

	m_device9->SetVertexDeclaration(m_vertexTexDecl);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertexTex));

	m_device9->SetVertexShader(nullptr);
	m_device9->SetPixelShader(nullptr);
}

void RenderTarget9::DrawBlurredSprite(const Utils::Vector2 &vPosition, std::shared_ptr<const UI::D3DTexture> pTexture,
									   const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor) {
	auto pTexture9 = std::dynamic_pointer_cast<const UI::D3DTextureObject9>(pTexture->GetObject());
	if (pTexture9 == nullptr)
		return;

	Utils::Color color(dwColor);
	float4 fColor = {
		color.R / 255.0f, color.G / 255.0f,
		color.B / 255.0f, color.A / 255.0f
	};

	SVertexTex vertices[] = {
		{ vPosition.x + dimensions[0].x, vPosition.y + dimensions[0].y, 0, 0, 0, fColor },
		{ vPosition.x + dimensions[1].x, vPosition.y + dimensions[1].y, 0, 1, 0, fColor },
		{ vPosition.x + dimensions[2].x, vPosition.y + dimensions[2].y, 0, 1, 1, fColor },
		{ vPosition.x + dimensions[3].x, vPosition.y + dimensions[3].y, 0, 0, 1, fColor }
	};

	m_device9->SetVertexShader(m_blurVertex);
	m_device9->SetPixelShader(m_blurPixel);
	m_device9->SetTexture(0, pTexture9->GetD3DTexture9());

	D3DVIEWPORT9 viewPort = {0};
	m_device9->GetViewport(&viewPort);
	float fScreenWidth = (float) viewPort.Width;
	float fScreenHeight = (float) viewPort.Height;
	
	D3DXMATRIX orthoProj;
	D3DXMatrixOrthoLH(&orthoProj, fScreenWidth, fScreenHeight, viewPort.MinZ, viewPort.MaxZ);
	m_device9->SetVertexShaderConstantF(0, orthoProj, 4);

	float fWidth = GetDimensionWidth(dimensions);
	float fHeight = GetDimensionHeight(dimensions);
	float params[] = { fWidth, fHeight, fScreenWidth, fScreenHeight };
	m_device9->SetPixelShaderConstantF(4, params, 1);
	m_device9->SetVertexShaderConstantF(4, params, 1);

	m_device9->SetVertexDeclaration(m_vertexTexDecl);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertexTex));

	m_device9->SetVertexShader(nullptr);
	m_device9->SetPixelShader(nullptr);
	m_device9->SetTexture(0, nullptr);
}

void RenderTarget9::DrawSprite(const Utils::Vector2 &vPosition, std::shared_ptr<const UI::D3DTexture> pTexture,
							   const std::array<Utils::Vector2, 4> &dimensions, uint32 dwColor) {
	auto pTexture9 = std::dynamic_pointer_cast<const UI::D3DTextureObject9>(pTexture->GetObject());
	if (pTexture9 == nullptr)
		return;

	SVertexTexFVF vertices[] = {
		{ vPosition.x + dimensions[0].x, vPosition.y + dimensions[0].y, 0, 1, dwColor, 0, 0 },
		{ vPosition.x + dimensions[1].x, vPosition.y + dimensions[1].y, 0, 1, dwColor, 1, 0 },
		{ vPosition.x + dimensions[2].x, vPosition.y + dimensions[2].y, 0, 1, dwColor, 1, 1 },
		{ vPosition.x + dimensions[3].x, vPosition.y + dimensions[3].y, 0, 1, dwColor, 0, 1 }
	};

	m_device9->SetTexture(0, pTexture9->GetD3DTexture9());
	m_device9->SetFVF(SVertexTexFVF::FVF);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertexTexFVF));

	m_device9->SetTexture(0, nullptr);
}

// Gradient
void RenderTarget9::FillRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
								  const std::array<Utils::Color, 4> &gradient) const {
	SVertex vertices[] = {
		{ vPosition.x + dimensions[0].x, vPosition.y + dimensions[0].y, 0, 1, gradient[0] },
		{ vPosition.x + dimensions[1].x, vPosition.y + dimensions[1].y, 0, 1, gradient[1] },
		{ vPosition.x + dimensions[2].x, vPosition.y + dimensions[2].y, 0, 1, gradient[2] },
		{ vPosition.x + dimensions[3].x, vPosition.y + dimensions[3].y, 0, 1, gradient[3] }
	};

	m_device9->SetFVF(SVertex::FVF);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertex));
}

void RenderTarget9::FillRoundedRectangle(const Utils::Vector2 &vPosition, const std::array<Utils::Vector2, 4> &dimensions,
										 const float4 &fHorizRadius, const float4 &fVertRadius, const std::array<Utils::Color, 4> &gradient) const {
	float4 fColor0 = {
		gradient[0].R / 255.0f, gradient[0].G / 255.0f,
		gradient[0].B / 255.0f, gradient[0].A / 255.0f
	};

	float4 fColor1 = {
		gradient[1].R / 255.0f, gradient[1].G / 255.0f,
		gradient[1].B / 255.0f, gradient[1].A / 255.0f
	};

	float4 fColor2 = {
		gradient[2].R / 255.0f, gradient[2].G / 255.0f,
		gradient[2].B / 255.0f, gradient[2].A / 255.0f
	};

	float4 fColor3 = {
		gradient[3].R / 255.0f, gradient[3].G / 255.0f,
		gradient[3].B / 255.0f, gradient[3].A / 255.0f
	};

	SVertexTex vertices[] = {
		{ vPosition.x + dimensions[0].x - 2, vPosition.y + dimensions[0].y - 2, 0, 0, 0, fColor0 },
		{ vPosition.x + dimensions[1].x + 2, vPosition.y + dimensions[1].y - 2, 0, 1, 0, fColor1 },
		{ vPosition.x + dimensions[2].x + 2, vPosition.y + dimensions[2].y + 2 , 0, 1, 1, fColor2 },
		{ vPosition.x + dimensions[3].x - 2, vPosition.y + dimensions[3].y + 2, 0, 0, 1, fColor3 }
	};

	m_device9->SetVertexShader(m_roundRectVertex);
	m_device9->SetPixelShader(m_roundRectFillPixel);

	D3DVIEWPORT9 viewPort = {0};
	m_device9->GetViewport(&viewPort);
	float fScreenWidth = (float) viewPort.Width;
	float fScreenHeight = (float) viewPort.Height;
	
	D3DXMATRIX orthoProj;
	D3DXMatrixOrthoLH(&orthoProj, fScreenWidth, fScreenHeight, viewPort.MinZ, viewPort.MaxZ);
	m_device9->SetVertexShaderConstantF(0, orthoProj, 4);
	
	float fWidth = GetDimensionWidth(dimensions) + 4;
	float fHeight = GetDimensionHeight(dimensions) + 4;
	float params1[] = { 1.0f, 1.0f / fWidth, 0, 0 };
	m_device9->SetPixelShaderConstantF(4, params1, 1);

	float radius[] = { fHorizRadius._1, fHorizRadius._2, fHorizRadius._3, fHorizRadius._4,
					   fVertRadius._1, fVertRadius._2, fVertRadius._3, fVertRadius._4 };
	m_device9->SetPixelShaderConstantF(6, radius, 2);

	float params2[] = { fWidth, fHeight, fScreenWidth, fScreenHeight };
	m_device9->SetPixelShaderConstantF(5, params2, 1);
	m_device9->SetVertexShaderConstantF(5, params2, 1);

	m_device9->SetVertexDeclaration(m_vertexTexDecl);
	m_device9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SVertexTex));

	m_device9->SetVertexShader(nullptr);
	m_device9->SetPixelShader(nullptr);
}


// StateBlock initialisation...
const float g_fOne = 1.0f;
const float g_fZero = 0.0f;
const float g_f64 = 64.0f;

const SStateBlockRecord<D3DRENDERSTATETYPE> RenderTarget9::m_renderStateTable[] =
{
	{ D3DRS_ZENABLE,                    FALSE                            }, // TRUE
	{ D3DRS_FILLMODE,                   D3DFILL_SOLID                    },
	{ D3DRS_SHADEMODE,                  D3DSHADE_GOURAUD                 },
	{ D3DRS_ZWRITEENABLE,               FALSE                            }, // TRUE
	{ D3DRS_ALPHATESTENABLE,            TRUE                             }, // FALSE
	{ D3DRS_LASTPIXEL,                  TRUE                             },
	{ D3DRS_SRCBLEND,                   D3DBLEND_SRCALPHA                }, // D3DBLEND_ONE
	{ D3DRS_DESTBLEND,                  D3DBLEND_INVSRCALPHA             }, // D3DBLEND_ZERO
	{ D3DRS_CULLMODE,                   D3DCULL_NONE                     }, // D3DCULL_CCW
	{ D3DRS_ZFUNC,                      D3DCMP_LESSEQUAL                 },
	{ D3DRS_ALPHAREF,                   10                               }, // 0
	{ D3DRS_ALPHAFUNC,                  D3DCMP_GREATEREQUAL              }, // D3DCMP_NONE
	{ D3DRS_DITHERENABLE,               FALSE                            },
	{ D3DRS_ALPHABLENDENABLE,           TRUE                             }, // FALSE
	{ D3DRS_FOGENABLE,                  FALSE                            },
	{ D3DRS_SPECULARENABLE,             FALSE                            },
	{ D3DRS_FOGCOLOR,                   0                                },
	{ D3DRS_FOGTABLEMODE,               D3DFOG_NONE                      },
	{ D3DRS_FOGSTART,                   *(DWORD*)&g_fZero                },
	{ D3DRS_FOGEND,                     *(DWORD*)&g_fZero                },
	{ D3DRS_FOGDENSITY,                 *(DWORD*)&g_fOne                 },
	{ D3DRS_RANGEFOGENABLE,             FALSE                            },
	{ D3DRS_STENCILENABLE,              FALSE                            },
	{ D3DRS_STENCILFAIL,                D3DSTENCILOP_KEEP                },
	{ D3DRS_STENCILZFAIL,               D3DSTENCILOP_KEEP                },
	{ D3DRS_STENCILPASS,                D3DSTENCILOP_KEEP                },
	{ D3DRS_STENCILFUNC,                D3DCMP_ALWAYS                    },
	{ D3DRS_STENCILREF,                 0                                },
	{ D3DRS_STENCILMASK,                0xFFFFFFFF                       },
	{ D3DRS_STENCILWRITEMASK,           0xFFFFFFFF                       },
	{ D3DRS_TEXTUREFACTOR,              0xFFFFFFFF                       },
	{ D3DRS_WRAP0,                      0                                },
	{ D3DRS_WRAP1,                      0                                },
	{ D3DRS_WRAP2,                      0                                },
	{ D3DRS_WRAP3,                      0                                },
	{ D3DRS_WRAP4,                      0                                },
	{ D3DRS_WRAP5,                      0                                },
	{ D3DRS_WRAP6,                      0                                },
	{ D3DRS_WRAP7,                      0                                },
	{ D3DRS_CLIPPING,                   FALSE                            }, // TRUE
	{ D3DRS_LIGHTING,                   FALSE                            }, // TRUE
	{ D3DRS_AMBIENT,                    0                                },
	{ D3DRS_FOGVERTEXMODE,              D3DFOG_NONE                      },
	{ D3DRS_COLORVERTEX,                TRUE                             },
	{ D3DRS_LOCALVIEWER,                TRUE                             },
	{ D3DRS_NORMALIZENORMALS,           FALSE                            },
	{ D3DRS_DIFFUSEMATERIALSOURCE,      D3DMCS_COLOR1                    },
	{ D3DRS_SPECULARMATERIALSOURCE,     D3DMCS_COLOR2                    },
	{ D3DRS_AMBIENTMATERIALSOURCE,      D3DMCS_MATERIAL                  },
	{ D3DRS_EMISSIVEMATERIALSOURCE,     D3DMCS_MATERIAL                  },
	{ D3DRS_VERTEXBLEND,                D3DVBF_DISABLE                   },
	{ D3DRS_CLIPPLANEENABLE,            FALSE                            },
	{ D3DRS_POINTSIZE,                  *(DWORD*)&g_fOne                 },
	{ D3DRS_POINTSIZE_MIN,              *(DWORD*)&g_fOne                 },
	{ D3DRS_POINTSPRITEENABLE,          FALSE                            },
	{ D3DRS_POINTSCALEENABLE,           FALSE                            },
	{ D3DRS_POINTSCALE_A,               *(DWORD*)&g_fOne                 },
	{ D3DRS_POINTSCALE_B,               *(DWORD*)&g_fZero                },
	{ D3DRS_POINTSCALE_C,               *(DWORD*)&g_fZero                },
	{ D3DRS_MULTISAMPLEANTIALIAS,       FALSE                            }, // TRUE
	{ D3DRS_MULTISAMPLEMASK,            0xFFFFFFFF                       },
	{ D3DRS_PATCHEDGESTYLE,             D3DPATCHEDGE_DISCRETE            },
	{ D3DRS_DEBUGMONITORTOKEN,          D3DDMT_ENABLE                    },
	{ D3DRS_POINTSIZE_MAX,              *(DWORD*)&g_f64                  },
	{ D3DRS_INDEXEDVERTEXBLENDENABLE,   FALSE                            },
	{ D3DRS_COLORWRITEENABLE,           0x0000000F                       },
	{ D3DRS_TWEENFACTOR,                *(DWORD*)&g_fOne                 },
	{ D3DRS_BLENDOP,                    D3DBLENDOP_ADD                   },
	{ D3DRS_POSITIONDEGREE,             D3DDEGREE_CUBIC                  },
	{ D3DRS_NORMALDEGREE,               D3DDEGREE_LINEAR                 },
	{ D3DRS_SCISSORTESTENABLE,          FALSE                            },
	{ D3DRS_SLOPESCALEDEPTHBIAS,        0                                },
	{ D3DRS_ANTIALIASEDLINEENABLE,      FALSE                            },
	{ D3DRS_MINTESSELLATIONLEVEL,       *(DWORD*)&g_fOne                 },
	{ D3DRS_MAXTESSELLATIONLEVEL,       *(DWORD*)&g_fOne                 },
	{ D3DRS_ADAPTIVETESS_X,             *(DWORD*)&g_fZero                },
	{ D3DRS_ADAPTIVETESS_Y,             *(DWORD*)&g_fZero                },
	{ D3DRS_ADAPTIVETESS_Z,             *(DWORD*)&g_fOne                 },
	{ D3DRS_ADAPTIVETESS_W,             *(DWORD*)&g_fZero                },
	{ D3DRS_ENABLEADAPTIVETESSELLATION, FALSE                            },
	{ D3DRS_TWOSIDEDSTENCILMODE,        FALSE                            },
	{ D3DRS_CCW_STENCILFAIL,            D3DSTENCILOP_KEEP                },
	{ D3DRS_CCW_STENCILZFAIL,           D3DSTENCILOP_KEEP                },
	{ D3DRS_CCW_STENCILPASS,            D3DSTENCILOP_KEEP                },
	{ D3DRS_CCW_STENCILFUNC,            D3DCMP_ALWAYS                    },
	{ D3DRS_COLORWRITEENABLE1,          0x0000000F                       },
	{ D3DRS_COLORWRITEENABLE2,          0x0000000F                       },
	{ D3DRS_COLORWRITEENABLE3,          0x0000000F                       },
	{ D3DRS_BLENDFACTOR,                0xFFFFFFFF                       },
	{ D3DRS_SRGBWRITEENABLE,            0                                },
	{ D3DRS_DEPTHBIAS,                  0                                },
	{ D3DRS_WRAP8,                      0                                },
	{ D3DRS_WRAP9,                      0                                },
	{ D3DRS_WRAP10,                     0                                },
	{ D3DRS_WRAP11,                     0                                },
	{ D3DRS_WRAP12,                     0                                },
	{ D3DRS_WRAP13,                     0                                },
	{ D3DRS_WRAP14,                     0                                },
	{ D3DRS_WRAP15,                     0                                },
	{ D3DRS_SEPARATEALPHABLENDENABLE,   FALSE                            },
	{ D3DRS_SRCBLENDALPHA,              D3DBLEND_ONE                     },
	{ D3DRS_DESTBLENDALPHA,             D3DBLEND_ZERO                    },
	{ D3DRS_BLENDOPALPHA,               D3DBLENDOP_ADD                   }
};

const SStateBlockRecord<D3DSAMPLERSTATETYPE> RenderTarget9::m_samplerStateTable[] =
{
	{ D3DSAMP_ADDRESSU,       D3DTADDRESS_CLAMP                          }, // D3DTADDRESS_WRAP
	{ D3DSAMP_ADDRESSV,       D3DTADDRESS_CLAMP                          }, // D3DTADDRESS_WRAP
	{ D3DSAMP_ADDRESSW,       D3DTADDRESS_CLAMP                          }, // D3DTADDRESS_WRAP
	{ D3DSAMP_BORDERCOLOR,    0x00000000                                 },
	{ D3DSAMP_MAGFILTER,      D3DTEXF_POINT                              },
	{ D3DSAMP_MINFILTER,      D3DTEXF_POINT                              },
	{ D3DSAMP_MIPFILTER,      D3DTEXF_NONE                               },
	{ D3DSAMP_MIPMAPLODBIAS,  0                                          },
	{ D3DSAMP_MAXMIPLEVEL,    0                                          },
	{ D3DSAMP_MAXANISOTROPY,  1                                          },
	{ D3DSAMP_SRGBTEXTURE,    0                                          },
	{ D3DSAMP_ELEMENTINDEX,   0                                          },
	{ D3DSAMP_DMAPOFFSET,     0                                          },
};

const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> RenderTarget9::m_textureStage1StateTable[] =
{
	{ D3DTSS_COLOROP,                D3DTOP_MODULATE                     },
	{ D3DTSS_COLORARG1,              D3DTA_TEXTURE                       },
	{ D3DTSS_COLORARG2,              D3DTA_DIFFUSE                       }, // D3DTA_CURRENT
	{ D3DTSS_ALPHAOP,                D3DTOP_MODULATE                     }, // D3DTOP_SELECTARG1
	{ D3DTSS_ALPHAARG1,              D3DTA_TEXTURE                       },
	{ D3DTSS_ALPHAARG2,              D3DTA_DIFFUSE                       }, // D3DTA_CURRENT
	{ D3DTSS_BUMPENVMAT00,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVMAT01,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVMAT10,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVMAT11,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_TEXCOORDINDEX,          0                                   },
	{ D3DTSS_BUMPENVLSCALE,          *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVLOFFSET,         *(DWORD*)&g_fZero                   },
	{ D3DTSS_TEXTURETRANSFORMFLAGS,  D3DTTFF_DISABLE                     },
	{ D3DTSS_COLORARG0,              D3DTA_CURRENT                       },
	{ D3DTSS_ALPHAARG0,              D3DTA_CURRENT                       },
	{ D3DTSS_RESULTARG,              D3DTA_CURRENT                       },
	{ D3DTSS_CONSTANT,               0 /* ??? */                         },
};

const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> RenderTarget9::m_textureStageNStateTable[] =
{
	{ D3DTSS_COLOROP,                D3DTOP_DISABLE                      },
	{ D3DTSS_COLORARG1,              D3DTA_TEXTURE                       },
	{ D3DTSS_COLORARG2,              D3DTA_CURRENT                       },
	{ D3DTSS_ALPHAOP,                D3DTOP_DISABLE                      },
	{ D3DTSS_ALPHAARG1,              D3DTA_TEXTURE                       },
	{ D3DTSS_ALPHAARG2,              D3DTA_CURRENT                       },
	{ D3DTSS_BUMPENVMAT00,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVMAT01,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVMAT10,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVMAT11,           *(DWORD*)&g_fZero                   },
	{ D3DTSS_TEXCOORDINDEX,          0                                   },
	{ D3DTSS_BUMPENVLSCALE,          *(DWORD*)&g_fZero                   },
	{ D3DTSS_BUMPENVLOFFSET,         *(DWORD*)&g_fZero                   },
	{ D3DTSS_TEXTURETRANSFORMFLAGS,  D3DTTFF_DISABLE                     },
	{ D3DTSS_COLORARG0,              D3DTA_CURRENT                       },
	{ D3DTSS_ALPHAARG0,              D3DTA_CURRENT                       },
	{ D3DTSS_RESULTARG,              D3DTA_CURRENT                       },
	{ D3DTSS_CONSTANT,               0                                   },
};

CComPtr<IDirect3DStateBlock9> RenderTarget9::_createStateBlock(D3DSTATEBLOCKTYPE type) const {
	CComPtr<IDirect3DStateBlock9> pStateBlock = nullptr;
	HRESULT hResult = m_device9->CreateStateBlock(type, &pStateBlock);
	assert(hResult == D3D_OK);
	return pStateBlock;
}

CComPtr<IDirect3DStateBlock9> RenderTarget9::_createDefaultStateBlock() const {
	m_device9->BeginStateBlock();

	// Apply default states
	for (const auto &cur: m_renderStateTable)
		m_device9->SetRenderState(cur.ordinal, cur.value);

	for (int i = 0; i < 4; ++i) {
		for (const auto &cur: m_samplerStateTable)
			m_device9->SetSamplerState(i, cur.ordinal, cur.value);
	}

	for (const auto &cur: m_textureStage1StateTable)
		m_device9->SetTextureStageState(0, cur.ordinal, cur.value);

	for (int i = 1; i < 7; ++i) {
		for (const auto &cur: m_textureStageNStateTable)
			m_device9->SetTextureStageState(i, cur.ordinal, cur.value);
	}

	// Set shaders and texture to null
	m_device9->SetPixelShader(nullptr);
	m_device9->SetVertexShader(nullptr);
	m_device9->SetTexture(0, nullptr);
	m_device9->SetVertexDeclaration(nullptr);

	CComPtr<IDirect3DStateBlock9> pStateBlock = nullptr;
	m_device9->EndStateBlock(&pStateBlock);
	return pStateBlock;
}
