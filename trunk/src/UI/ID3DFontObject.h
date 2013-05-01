#pragma once
#include "ID3DSprite.h"

namespace UI {
	struct SFontDesc {
		uint32 Height;
		uint32 Width;
		uint32 Weight;
		uint32 MipLevels;
		bool Italic;
		uint8 CharSet;
		uint8 OutputPrecision;
		uint8 Quality;
		uint8 PitchAndFamily;
		std::wstring FaceName;
	};

	class ID3DFontObject abstract
	{
	public:
		virtual ~ID3DFontObject() {}
		virtual void OnLostDevice() const {}
		virtual void OnResetDevice() const {}

		virtual void DrawText(const std::shared_ptr<const ID3DSprite> &pSprite, std::wstring swText,
			RECT &screenRect, uint32 dwFlags, const Utils::Color &color) const = 0;

		virtual RECT GetTextExtent(std::wstring swText, RECT rect, uint32 flags) const = 0;
		virtual RECT CalcTextRect(std::wstring swText, uint32 begin, uint32 end, RECT rect) const = 0;
		virtual POINT GetGlyphCellInc(wchar_t c) const = 0;
	};
}
