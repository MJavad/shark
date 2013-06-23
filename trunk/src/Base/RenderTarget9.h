#pragma once
#include "IRenderTarget.h"
#include "UI/D3DSurface9.h"

template <typename T>
struct SStateBlockRecord {
	T ordinal;
	DWORD value;
};

class RenderTarget9 : public IRenderTarget
{
public:
	RenderTarget9(IDirect3DDevice9 *pDevice);
	virtual void BeginUI();
	virtual void EndUI();
	virtual void OnLostDevice();
	virtual void OnResetDevice();
	virtual bool GetSurfaceRect(RECT *pRect) const;
	virtual bool GetClippingArea(RECT *pRect) const;
	virtual void SetClippingArea(const RECT *pRect) const;

	virtual std::shared_ptr<UI::D3DTexture> CreateRenderTargetTexture(uint32 uWidth, uint32 uHeight) const;
	virtual std::shared_ptr<UI::ID3DSurface> CreateRenderTargetSurface(uint32 uWidth, uint32 uHeight) const;
	virtual void SetRenderTargetSurface(const std::shared_ptr<const UI::ID3DSurface> &pSurface, uint32 uIndex = 0, bool bClear = false);
	virtual std::shared_ptr<UI::ID3DSurface> GetRenderTargetSurface(uint32 uIndex = 0) const;

	virtual void DrawRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient,
		float fStroke) const;

	virtual void FillRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawRoundedRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius,
		const float4 &fVertRadius,
		const std::array<D3DXCOLOR, 4> &gradient,
		float fStroke) const;

	virtual void FillRoundedRectangle(const Utils::Vector2 &vPosition,
		const std::array<Utils::Vector2, 4> &dimensions,
		const float4 &fHorizRadius,
		const float4 &fVertRadius,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawBlurredSprite(const Utils::Vector2 &vPosition,
		std::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

	virtual void DrawSprite(const Utils::Vector2 &vPosition,
		std::shared_ptr<const UI::D3DTexture> pTexture,
		const std::array<Utils::Vector2, 4> &dimensions,
		const std::array<D3DXCOLOR, 4> &gradient) const;

protected:
	IDirect3DDevice9 *m_device9;
	CComPtr<IDirect3DStateBlock9> m_stateBlock;
	CComPtr<IDirect3DStateBlock9> m_defaultStateBlock;
	CComPtr<IDirect3DStateBlock9> _createStateBlock(D3DSTATEBLOCKTYPE type) const;
	CComPtr<IDirect3DStateBlock9> _createDefaultStateBlock() const;

	// Stateblock default data
	static const SStateBlockRecord<D3DRENDERSTATETYPE> m_renderStateTable[];
	static const SStateBlockRecord<D3DSAMPLERSTATETYPE> m_samplerStateTable[];
	static const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> m_textureStage1StateTable[];
	static const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> m_textureStageNStateTable[];

	// Vertex Declaration
	CComPtr<IDirect3DVertexDeclaration9> m_vertexTexDecl;

	// Rounded Rectangle Shaders
	CComPtr<IDirect3DVertexShader9> m_roundRectVertex;
	CComPtr<IDirect3DPixelShader9> m_roundRectDrawPixel, m_roundRectFillPixel;

	// Blur Shaders
	CComPtr<IDirect3DVertexShader9> m_blurVertex;
	CComPtr<IDirect3DPixelShader9> m_blurPixel;
};
