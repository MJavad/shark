#pragma once

namespace UI {
	class ID3DSurface abstract
	{
	public:
		virtual ~ID3DSurface() {}
		virtual HDC GetDC() const = 0;
	};
}
