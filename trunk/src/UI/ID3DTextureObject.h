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
