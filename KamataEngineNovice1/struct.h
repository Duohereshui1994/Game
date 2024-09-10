#pragma once
#include <cmath>
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


typedef struct Vector2 {
	float x;
	float y;
public:
	// 数式
	bool operator==(const Vector2& other) { return x == other.x && y == other.y; }
	bool operator!=(const Vector2& other) { return x != other.x && y != other.y; }
	Vector2& operator+=(const Vector2& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	Vector2& operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}
	Vector2& operator*=(const int scalar) {
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}
	Vector2 operator+(const Vector2 other) const { return Vector2(x + other.x, y + other.y); }
	Vector2 operator-(const Vector2 other) const { return Vector2{ x - other.x, y - other.y }; }
	Vector2 operator*(const Vector2 other) const { return Vector2{ x * other.x, y * other.y }; }
	Vector2 operator*(const float scalar) const { return Vector2{ x * scalar, y * scalar }; }
	Vector2 operator*(const int scalar) const { return Vector2{ x * scalar, y * scalar }; }
	friend Vector2 operator*(const float scalar, const Vector2& v) { return Vector2{ scalar * v.x, scalar * v.y }; }
	friend Vector2 operator*(const int scalar, const Vector2& v) { return Vector2{ scalar * v.x, scalar * v.y }; }

	// 正規化
	Vector2 Normalize() {
		float length = sqrtf(powf(x, 2) + powf(y, 2));
		return Vector2(x / length, y / length);
	}
	// 長さ
	float Length() { return sqrtf(x * x + y * y); }
	// 内積
	float Dot(const Vector2& v) const { return x * v.x + y * v.y; }
	// クロス積(外積)、ベクトル２の外積の戻り値はfloat
	float Cross(const Vector2& v) const { return x * v.y - y * v.x; }
}Vector2;

typedef struct Matrix3x3 {

	float m[3][3];

} Matrix3x3;

typedef struct Point {
	float left;
	float top;
	float right;
	float bottom;
}Point;

typedef struct Line {
	Vector2 begin;
	Vector2 end;
}Line;

typedef struct Vertex {
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
}Vertex;

typedef struct Affine {
	Vector2 scale;
	float theta;
	Vector2 translate;
}Affine;

typedef struct LeftAndSize {
	float left;
	float top;
	float width;
	float height;
}LeftAndSize;

typedef struct LinePosition {
	Line lateral;
	Line vert;
}LinePosition;

typedef struct Speed {
	float scale;
	float rotate;
	float translate;
}Speed;
