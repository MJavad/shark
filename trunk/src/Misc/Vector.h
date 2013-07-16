#pragma once

namespace Utils
{
	class Vector2;
	class Vector3;

	class Vector2
	{
	public:
		explicit Vector2(LPARAM lParam) : x(0), y(0) {
			x = static_cast<float>(GET_X_LPARAM(lParam));
			y = static_cast<float>(GET_Y_LPARAM(lParam));
		}

		Vector2(const float &x = 0.0f, const float &y = 0.0f) : x(x), y(y) { }
		Vector2(const D3DXVECTOR2 &vec) : x(vec.x), y(vec.y) { }
		Vector2(const D3DXVECTOR3 &vec) : x(vec.x), y(vec.y) { }

		operator D3DXVECTOR2() const { return D3DXVECTOR2(x, y); }
		operator D3DXVECTOR3() const { return D3DXVECTOR3(x, y, 0.0f); }

		operator POINT() const {
			POINT pt = {0};
			pt.x = static_cast<LONG>(x);
			pt.y = static_cast<LONG>(y);
			return pt;
		}

		Vector2 operator + (const Vector2 &loc) const {
			Vector2 ret;
			ret.x = x + loc.x;
			ret.y = y + loc.y;
			return ret;
		}

		Vector2 operator - (const Vector2 &loc) const {
			Vector2 ret;
			ret.x = x - loc.x;
			ret.y = y - loc.y;
			return ret;
		}

		Vector2 operator * (const Vector2 &loc) const {
			Vector2 ret;
			ret.x = x * loc.x;
			ret.y = y * loc.y;
			return ret;
		}

		Vector2 operator / (const Vector2 &loc) const {
			Vector2 ret;
			ret.x = x / loc.x;
			ret.y = y / loc.y;
			return ret;
		}

		Vector2& operator += (const Vector2 &loc) {
			x += loc.x;
			y += loc.y;
			return *this;
		}

		Vector2& operator -= (const Vector2 &loc) {
			x -= loc.x;
			y -= loc.y;
			return *this;
		}

		Vector2& operator *= (const Vector2 &loc) {
			x *= loc.x;
			y *= loc.y;
			return *this;
		}

		Vector2& operator /= (const Vector2 &loc) {
			x /= loc.x;
			y /= loc.y;
			return *this;
		}

		bool operator == (const Vector2 &loc) const {
			return (x == loc.x) && (y == loc.y);
		}

		bool operator != (const Vector2 &loc) const {
			return (x != loc.x) || (y != loc.y);
		}

		bool operator < (const Vector2 &other) const {
			return (x < other.x) || (y < other.y);
		}

		bool operator > (const Vector2 &other) const {
			return (x > other.x) || (y > other.y);
		}

		operator bool() const {
			return (x != 0) || (y != 0);
		}

		float distance_to(const Vector2 &loc) const {
			return std::sqrt(std::pow(x - loc.x, 2) + std::pow(y - loc.y, 2));
		}

		static float GetDistance(const Vector2 &loc1, const Vector2 &loc2) {
			return std::sqrt(std::pow(loc1.x - loc2.x, 2) + std::pow(loc1.y - loc2.y, 2));
		}

		float length() const {
			return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
		}

		Vector2 normalize() const {
			float fLength = length();
			if (fLength == 0.0f)
				return *this;

			return Vector2(x / fLength, y / fLength);
		}

		Vector2 floor() const {
			return Vector2(std::floor(x), std::floor(y));
		}

		float dot(const Vector2 &vOther) const {
			return (x * vOther.x + y * vOther.y);
		}

		Vector2 round_to(size_t uDigits) const {
			float fMultiplicator = std::pow(10.0f, float(uDigits));
			float fNewX = std::floor(x * fMultiplicator + 0.5f) / 100.0f;
			float fNewY = std::floor(y * fMultiplicator + 0.5f) / 100.0f;
			return Vector2(fNewX, fNewY);
		}

		Vector2 rotate_transform(const Vector2 &vPoint, float fAngle) const {
			Vector2 vResult;
			Vector2 vRelative = *this - vPoint;
			vResult.x = vRelative.x * std::cos(fAngle) - vRelative.y * std::sin(fAngle);
			vResult.y = vRelative.x * std::sin(fAngle) + vRelative.y * std::cos(fAngle);
			return vResult;
		}

		float angle() const {
			float fAngle = std::atan2(y, x);
			if (fAngle < 0.0f)
				fAngle += M_TWO_PI_F;

			return fAngle;
		}

		float x, y;
	};

	class Vector3
	{
	public:
		explicit Vector3(LPARAM lParam) : x(0), y(0), z(0) {
			x = static_cast<float>(GET_X_LPARAM(lParam));
			y = static_cast<float>(GET_Y_LPARAM(lParam));
		}

		Vector3(const float &x = 0.0f,
				const float &y = 0.0f,
				const float &z = 0.0f) : x(x), y(y), z(z) { }

		Vector3(const Vector2 &vec) : x(vec.x), y(vec.y), z(0.0f) { }
		Vector3(const D3DXVECTOR2 &vec) : x(vec.x), y(vec.y), z(0.0f) { }
		Vector3(const D3DXVECTOR3 &vec) : x(vec.x), y(vec.y), z(vec.z) { }

		operator Vector2() const { return Vector2(x, y); }
		operator D3DXVECTOR2() const { return D3DXVECTOR2(x, y); }
		operator D3DXVECTOR3() const { return D3DXVECTOR3(x, y, z); }

		operator POINT() const {
			POINT pt = {0};
			pt.x = static_cast<LONG>(x);
			pt.y = static_cast<LONG>(y);
			return pt;
		}

		Vector3 operator + (const Vector3 &loc) const {
			Vector3 ret;
			ret.x = x + loc.x;
			ret.y = y + loc.y;
			ret.z = z + loc.z;
			return ret;
		}

		Vector3 operator - (const Vector3 &loc) const {
			Vector3 ret;
			ret.x = x - loc.x;
			ret.y = y - loc.y;
			ret.z = z - loc.z;
			return ret;
		}

		Vector3 operator * (const Vector3 &loc) const {
			Vector3 ret;
			ret.x = x * loc.x;
			ret.y = y * loc.y;
			ret.z = z * loc.z;
			return ret;
		}

		Vector3 operator / (const Vector3 &loc) const {
			Vector3 ret;
			ret.x = x / loc.x;
			ret.y = y / loc.y;
			ret.z = z / loc.z;
			return ret;
		}

		Vector3& operator += (const Vector3 &loc) {
			x += loc.x;
			y += loc.y;
			z += loc.z;
			return *this;
		}

		Vector3& operator -= (const Vector3 &loc) {
			x -= loc.x;
			y -= loc.y;
			z -= loc.z;
			return *this;
		}

		Vector3& operator *= (const Vector3 &loc) {
			x *= loc.x;
			y *= loc.y;
			z *= loc.z;
			return *this;
		}

		Vector3& operator /= (const Vector3 &loc) {
			x /= loc.x;
			y /= loc.y;
			z /= loc.z;
			return *this;
		}

		bool operator == (const Vector3 &loc) const {
			return (x == loc.x) && (y == loc.y) && (z == loc.z);
		}

		bool operator != (const Vector3 &loc) const {
			return (x != loc.x) || (y != loc.y) || (z != loc.z);
		}

		bool operator < (const Vector3 &other) const {
			return (x < other.x) || (y < other.y) || (z < other.z);
		}

		bool operator > (const Vector3 &other) const {
			return (x > other.x) || (y > other.y) || (z > other.z);
		}

		operator bool() const {
			return (x != 0) || (y != 0) || (z != 0);
		}

		float distance_to(const Vector3 &loc) const {
			return std::sqrt(std::pow(x - loc.x, 2) +
							 std::pow(y - loc.y, 2) +
							 std::pow(z - loc.z, 2));
		}

		static float GetDistance(const Vector3 &loc1, const Vector3 &loc2) {
			return std::sqrt(std::pow(loc1.x - loc2.x, 2) +
							 std::pow(loc1.y - loc2.y, 2) +
							 std::pow(loc1.z - loc2.z, 2));
		}

		float length2D() const {
			return std::sqrt(std::pow(x, 2) +
							 std::pow(y, 2));
		}

		float length() const {
			return std::sqrt(std::pow(x, 2) +
							 std::pow(y, 2) +
							 std::pow(z, 2));
		}

		Vector3 normalize() const {
			float fLength = length();
			if (fLength == 0.0f)
				return *this;

			return Vector3(x / fLength, y / fLength, z / fLength );
		}

		Vector3 floor() const {
			return Vector3(std::floor(x),
						   std::floor(y),
						   std::floor(z));
		}

		Vector3 cross(const Vector3 &vOther) const {
			return Vector3(
				y * vOther.z - z * vOther.y,
				z * vOther.x - x * vOther.z,
				x * vOther.y - y * vOther.x);
		}

		float dot(const Vector3 &vOther) const {
			return (x * vOther.x + y * vOther.y + z * vOther.z);
		}

		Vector3 round_to(size_t uDigits) const {
			float fMultiplicator = std::pow(10.0f, float(uDigits));
			float fNewX = std::floor(x * fMultiplicator + 0.5f) / 100.0f;
			float fNewY = std::floor(y * fMultiplicator + 0.5f) / 100.0f;
			float fNewZ = std::floor(z * fMultiplicator + 0.5f) / 100.0f;
			return Vector3(fNewX, fNewY, fNewZ);
		}

		Vector3 rotate_transform(const Vector3 &vPoint, float fAngle) const {
			Vector3 vResult;
			Vector3 vRelative = *this - vPoint;
			vResult.x = vRelative.x * std::cos(fAngle) - vRelative.y * std::sin(fAngle);
			vResult.y = vRelative.x * std::sin(fAngle) + vRelative.y * std::cos(fAngle);
			vResult.z = vRelative.z;
			return vResult;
		}

		float angle() const {
			float fAngle = std::atan2(y, x);
			if (fAngle < 0.0f)
				fAngle += M_TWO_PI_F;

			return fAngle;
		}

		float x, y, z;
	};
}
