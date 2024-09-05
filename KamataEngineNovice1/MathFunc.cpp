#include "MathFunc.h"

Matrix3x3 MathFunc::MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate)
{
	Matrix3x3 result;
	result.m[0][0] = scale.x * cosf(rotate);
	result.m[0][1] = scale.x * sinf(rotate);
	result.m[0][2] = 0;
	result.m[1][0] = -scale.y * sinf(rotate);
	result.m[1][1] = scale.y * cosf(rotate);
	result.m[1][2] = 0;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1;
	return result;
}

Matrix3x3 MathFunc::Inverse(Matrix3x3 matrix)
{
	Matrix3x3 result = {};
	float A = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];
	if (A != 0) {
		result.m[0][0] = 1.0f / A * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
		result.m[0][1] = 1.0f / A * (matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]) * -1;
		result.m[0][2] = 1.0f / A * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);
		result.m[1][0] = 1.0f / A * (matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]) * -1;
		result.m[1][1] = 1.0f / A * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
		result.m[1][2] = 1.0f / A * (matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]) * -1;
		result.m[2][0] = 1.0f / A * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
		result.m[2][1] = 1.0f / A * (matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]) * -1;
		result.m[2][2] = 1.0f / A * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);
	}
	return result;
}

Matrix3x3 MathFunc::MakeOrthographicMatrix(float left, float top, float right, float bottom)
{
	Matrix3x3 result = {};
	result.m[0][0] = 2.0f / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[1][2] = 0;
	result.m[2][0] = (left + right) / (left - right);
	result.m[2][1] = (top + bottom) / (bottom - top);
	result.m[2][2] = 1;
	return result;
}

Matrix3x3 MathFunc::MakeViewportMatrix(float left, float top, float width, float height)
{
	Matrix3x3 result = {};
	result.m[0][0] = width / 2.0f;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = (height / 2.0f) * -1;
	result.m[1][2] = 0;
	result.m[2][0] = left + width / 2.0f;
	result.m[2][1] = top + height / 2.0f;
	result.m[2][2] = 1;
	return result;
}

Vector2 MathFunc::Transform(Vector2 vector, Matrix3x3 matrix)
{
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Matrix3x3 MathFunc::Multiply(Matrix3x3 Matrix1, Matrix3x3 Matrix2)
{
	Matrix3x3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = 0; // 初始化结果矩阵的元素为0
			for (int k = 0; k < 3; k++) {
				result.m[i][j] += Matrix1.m[i][k] * Matrix2.m[k][j];
			}
		}
	}
	return result;

}

Vector2 MathFunc::WorldToScreen(Vector2 worldTranslate,Vector2 cameraScale, float cameraRotate, Vector2 cameraTranslate, Vector2 worldPos)
{
	Matrix3x3 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldTranslate);
	Matrix3x3 cameraWorldMatrix = MakeAffineMatrix(cameraScale, cameraRotate, cameraTranslate);

	Matrix3x3 viewMatrix = Inverse(cameraWorldMatrix);
	Matrix3x3 orthoMatrix = MakeOrthographicMatrix(-640.0f, 360.0f, 640.0f, -360.0f);
	Matrix3x3 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f);

	Matrix3x3 wvpVpMatrix = Multiply(worldMatrix, viewMatrix);
	wvpVpMatrix = Multiply(wvpVpMatrix, orthoMatrix);
	wvpVpMatrix = Multiply(wvpVpMatrix, viewPortMatrix);
	Vector2 screenPos = Transform(worldPos, wvpVpMatrix);

	return screenPos;
}
Vector2 MathFunc::Normalize(const Vector2& v)
{
	float length = Length(v);
	if (length != 0)
	{
		Vector2 result = { 0,0 };
		result.x = v.x / length;
		result.y = v.y / length;
		return result;
	}
	else
	{
		Vector2 result = { 0,0 };
		return result;
	}
}


