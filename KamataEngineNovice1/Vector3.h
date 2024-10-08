#pragma once
#include <cmath>

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final { // final:継承禁止
	float x;
	float y;
	float z;

public:
	// 数式
	bool operator==(const Vector3& other) { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const Vector3& other) { return x != other.x && y != other.y && z != other.z; }
	Vector3& operator+=(const Vector3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}
	Vector3& operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}
	Vector3& operator*=(const int scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}
	Vector3 operator+(const Vector3 other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
	Vector3 operator-(const Vector3 other) const { return Vector3{ x - other.x, y - other.y, z - other.z }; }
	Vector3 operator*(const Vector3 other) const { return Vector3{ x * other.x, y * other.y, z * other.z }; }
	Vector3 operator*(const float scalar) const { return Vector3{ x * scalar, y * scalar, z * scalar }; }
	Vector3 operator*(const int scalar) const { return Vector3{ x * scalar, y * scalar, z * scalar }; }
	friend Vector3 operator*(const float scalar, const Vector3& v) { return Vector3{ scalar * v.x, scalar * v.y, scalar * v.z }; }
	friend Vector3 operator*(const int scalar, const Vector3& v) { return Vector3{ scalar * v.x, scalar * v.y, scalar * v.z }; }

	// 正規化
	Vector3 Normalize() {
		float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
		return Vector3(x / length, y / length, z / length);
	}
	// 長さ
	float Length() { return sqrtf(x * x + y * y + z * z); }
	// 内積
	float Dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
	// クロス積(外積)
	Vector3 Cross(const Vector3& v) const {
		Vector3 result{};
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;
		return result;
	}
};
