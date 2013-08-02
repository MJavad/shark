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
		uint32 faceNameHash;
	};

	class ID3DFontObject abstract : public virtual Utils::IDynamicObject
	{
	public:
		virtual ~ID3DFontObject() {}
		virtual void OnLostDevice() const {}
		virtual void OnResetDevice() const {}

		virtual void DrawText(const boost::shared_ptr<const ID3DSprite> &pSprite,
			std::wstring textString, RECT &screenRect, uint32 flags, const D3DXCOLOR &color) const = 0;

		virtual RECT GetTextExtent(std::wstring textString, RECT rect, uint32 flags) const = 0;
		virtual POINT GetGlyphCellInc(wchar_t c) const = 0;
	};
}
