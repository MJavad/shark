#pragma once
#pragma warning(disable: 4244)

namespace Utils
{
	class Color
	{
	public:
		Color() : A(0), R(0), G(0), B(0) {}
		Color( uint32 dwColor ) {
			A = (dwColor >> 24) & 0xFF;
			R = (dwColor >> 16) & 0xFF;
			G = (dwColor >> 8) & 0xFF;
			B = (dwColor >> 0) & 0xFF;
		}

		operator uint32() const {
			return (A << 24) | (R << 16) | (G << 8) | (B << 0);
		}

		Color& operator=( uint32 dwColor ) {
			A = (dwColor >> 24) & 0xFF;
			R = (dwColor >> 16) & 0xFF;
			G = (dwColor >> 8) & 0xFF;
			B = (dwColor >> 0) & 0xFF;
			return *this;
		}

		Color& operator+=( const Color &color ) {
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

		Color& operator-=( const Color &color ) {
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

		Color operator+( const Color &color ) const {
			Color result( *this );
			return result += color;
		}

		Color operator-( const Color &color ) const {
			Color result( *this );
			return result -= color;
		}

		static Color mix(const Color &src, const Color &dst, float pct) {
			return pct * dst + (1 - pct) * src;
		}

		uint8 A, R, G, B;
	};
}

#pragma warning(default: 4244)
