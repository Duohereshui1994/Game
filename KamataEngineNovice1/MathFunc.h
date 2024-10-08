#pragma once
#include "struct.h"
#include <assert.h>
#include <cmath>
#include <corecrt_math_defines.h>
class MathFunc
{
public:
	//物体对象的性质的结构体
	typedef struct {
		Vector2 position;
		Vector2 center;
		Vector2 scale;
		float rotate;
		float width;
		float height;
	}Object;


	//制作affine变化矩阵
	Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate);
	Matrix3x3 MakeAffineMatrix(Affine affine);
	Matrix3x3 MakeAffine(Affine affine);

	//视野矩阵
	Matrix3x3 ViewMatrix(Matrix3x3 cameraMatrix);

	//制作逆矩阵
	Matrix3x3 Inverse(Matrix3x3 matrix);

	//制作正交矩阵
	Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);
	Matrix3x3 MakeOrthographicMatrix(Point vertex);
	Matrix3x3 OrthoMatrix(Point vertex);

	//制作视口矩阵
	Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height);
	Matrix3x3 ViewportMatrix(LeftAndSize viewport);

	//制作wvpvp
	Matrix3x3 MakeVpVpMatrix(Matrix3x3 viewMatrix, Matrix3x3 orthoMatrix, Matrix3x3 viewportMatrix);
	Matrix3x3 WvpVpMatrix(Matrix3x3 worldMatrix, Matrix3x3 vpVpMatrix);

	//坐标轴的transform
	LinePosition TransformLine(LinePosition local, Matrix3x3 vpVpMatrix);

	//图像的四个点的变换
	Vertex TransformSprite(Vertex local, Matrix3x3 wvpVpMatrix);

	//坐标转换
	Vector2 Transform(Vector2 vector, Matrix3x3 matrix);

	//矩阵乘法
	Matrix3x3 Multiply(Matrix3x3 Matrix1, Matrix3x3 Matrix2);

	//世界坐标转化为屏幕坐标 （我感觉用不上，没有设定摄像机）
	Vector2 WorldToScreen(float rotate, Vector2 worldTranslate, Vector2 cameraScale, float cameraRotate, Vector2 cameraTranslate, Vector2 worldPos);

	//求向量长度
	float Length(const Vector2& v) {
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	//向量归一化
	Vector2 Normalize(const Vector2& v);

	//加算
	inline Vector2 Add(const Vector2& v1, const Vector2& v2) {
		Vector2 result = { 0,0 };
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		return result;
	}

	//減算
	inline Vector2 Subtract(const Vector2& v1, const Vector2& v2) {
		Vector2 result = { 0,0 };
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		return result;
	}

	//スカラー倍
	Vector2 Multiply(float Scaler, const Vector2& v) {
		Vector2 result = { 0,0 };
		result.x = Scaler * v.x;
		result.y = Scaler * v.y;
		return result;
	}

	//线性补间
	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) {
		return Add(Multiply((1.0f - t), v1), Multiply(t, v2));
	}

};

