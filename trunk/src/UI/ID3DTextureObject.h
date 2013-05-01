#pragma once
#include "ID3DSurface.h"

namespace UI {
	enum eTextureType {
		TEXTURE_FROM_FILE,
		TEXTURE_FROM_RESOURCE
	};

	struct STextureDesc {
		eTextureType TextureType;
		std::wstring FilePathOrResource;
		HMODULE ResourceModule;
		uint32 Width, Height;
		uint32 MipLevels;
		uint32 Usage;
		D3DFORMAT Format;
		D3DPOOL Pool;
		uint32 Filter;
		uint32 MipFilter;
		Utils::Color ColorKey;
	};

	class ID3DTextureObject abstract
	{
	public:
		virtual ~ID3DTextureObject() {}
		virtual std::shared_ptr<ID3DSurface> QuerySurface(uint32 uLevel) const = 0;
	};
}
