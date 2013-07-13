#pragma once
#include "ID3DSprite.h"

namespace UI {
	struct FontDescription {
		uint32 height;
		uint32 width;
		uint32 weight;
		uint32 mipLevels;
		bool italic;
		uint8 charSet;
		uint8 outputPrecision;
		uint8 quality;
		uint8 pitchAndFamily;
		std::wstring faceName;
	};

	class ID3DFontObject abstract
	{
	public:
		virtual ~ID3DFontObject() {}
		virtual void OnLostDevice() const {}
		virtual void OnResetDevice() const {}

		virtual void DrawText(const std::shared_ptr<const ID3DSprite> &pSprite, std::wstring swText,
			RECT &screenRect, uint32 dwFlags, const D3DXCOLOR &color) const = 0;

		virtual RECT GetTextExtent(std::wstring swText, RECT rect, uint32 flags) const = 0;
		virtual POINT GetGlyphCellInc(wchar_t c) const = 0;
	};
}
