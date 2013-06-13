#pragma once

namespace Utils
{
	class Color
	{
	public:
		Color() : A(0), R(0), G(0), B(0) {}

		Color(uint8 a, uint8 r,
			  uint8 g, uint8 b) :
			A(a), R(r), G(g), B(b) {}

		Color(uint32 dwColor) {
			A = (dwColor >> 24) & 0xFF;
			R = (dwColor >> 16) & 0xFF;
			G = (dwColor >> 8) & 0xFF;
			B = (dwColor >> 0) & 0xFF;
		}

		operator uint32() const {
			return (A << 24) | (R << 16) | (G << 8) | (B << 0);
		}

		Color& operator=(uint32 dwColor) {
			A = (dwColor >> 24) & 0xFF;
			R = (dwColor >> 16) & 0xFF;
			G = (dwColor >> 8) & 0xFF;
			B = (dwColor >> 0) & 0xFF;
			return *this;
		}

		Color& operator+=(const Color &color) {
			uint16 currA = A + color.A;
			uint16 currR = R + color.R;
			uint16 currG = G + color.G;
			uint16 currB = B + color.B;

			currA > 0xFF ? A = 0xFF : A += color.A;
			currR > 0xFF ? R = 0xFF : R += color.R;
			currG > 0xFF ? G = 0xFF : G += color.G;
			currB > 0xFF ? B = 0xFF : B += color.B;
			return *this;
		}

		Color& operator-=(const Color &color) {
			int16 currA = A - color.A;
			int16 currR = R - color.R;
			int16 currG = G - color.G;
			int16 currB = B - color.B;

			currA < 0x00 ? A = 0x00 : A -= color.A;
			currR < 0x00 ? R = 0x00 : R -= color.R;
			currG < 0x00 ? G = 0x00 : G -= color.G;
			currB < 0x00 ? B = 0x00 : B -= color.B;
			return *this;
		}

		Color operator+(const Color &color) const {
			Color result(*this);
			return result += color;
		}

		Color operator-(const Color &color) const {
			Color result(*this);
			return result -= color;
		}

		static Color Interpolate(const Color &src, const Color &dst, float pct) {
			assert(pct >= 0.0f && pct <= 1.0f);
			return static_cast<uint32>(pct * dst + (1 - pct) * src);
		}

		static Color SinusInterpolate(const Color &src, const Color &dst, float pct) {
			float sinval = sin(pct * 3.1415926f / 2.0f);
			return static_cast<uint32>(sinval * dst + (1 - sinval) * src);
		}

		uint8 A, R, G, B;
	};
}
