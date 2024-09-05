#pragma once
#include <Vector2.h>
#include <Matrix3x3.h>
#include <assert.h>
#include <cmath>
#include <corecrt_math_defines.h>
class MathFunc
{
public:
	typedef struct {
		Vector2 position;
		Vector2 center;
		Vector2 scale;
		float rotate;
		float width;
		float height;
	}Object;

	Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate);
	Matrix3x3 Inverse(Matrix3x3 matrix);
	Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);
	Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height);

	Vector2 Transform(Vector2 vector, Matrix3x3 matrix);
	Matrix3x3 Multiply(Matrix3x3 Matrix1, Matrix3x3 Matrix2);

	Vector2 WorldToScreen(Vector2 worldTranslate, Vector2 cameraScale, float cameraRotate, Vector2 cameraTranslate, Vector2 worldPos);

	float Length(const Vector2& v) {
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	Vector2 Normalize(const Vector2& v);

	/// <summary>
	/// 加算
	/// </summary>
	inline Vector2 Add(const Vector2& v1, const Vector2& v2) {
		Vector2 result = { 0,0 };
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		return result;
	}

	/// <summary>
	/// 減算
	/// </summary>
	inline Vector2 Subtract(const Vector2& v1, const Vector2& v2) {
		Vector2 result = { 0,0 };
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		return result;
	}

	/// <summary>
	/// スカラー倍
	/// </summary>
	Vector2 Multiply(float Scaler, const Vector2& v) {
		Vector2 result = { 0,0 };
		result.x = Scaler * v.x;
		result.y = Scaler * v.y;
		return result;
	}

};

