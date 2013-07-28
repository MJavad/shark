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
#include "ID3DSurface.h"

namespace UI {
	enum TextureType {
		TEXTURE_FROM_FILE,
		TEXTURE_FROM_RESOURCE
	};

	struct TextureDescription {
		TextureType type;
		std::wstring filePath;
		uint32 filePathHash;
		uint32 resourceId;
		HMODULE resourceModule;
		uint32 width, height;
		uint32 mipLevels;
		uint32 usage;
		D3DFORMAT format;
		D3DPOOL pool;
		uint32 filter;
		uint32 mipFilter;
		D3DXCOLOR colorKey;
	};

	class ID3DTextureObject abstract
	{
	public:
		virtual ~ID3DTextureObject() {}
		virtual boost::shared_ptr<ID3DSurface> QuerySurface(uint32 uLevel) const = 0;
	};
}
