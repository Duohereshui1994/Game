#pragma once
#include "MathFunc.h"
class Camera
{
private:

#pragma region 定義しないといけない
	//カメラポジションの 拡縮・回転・移動
	Affine affine_;
	//切り取る範囲の左上と右上
	Point vertex_;
	//ラインの座標(ローカル座標)
	LinePosition local_;
	//カメラのスピード
	Speed speed_;

	MathFunc* math_ = nullptr;
#pragma endregion

	//ラインの座標(スクリーン座標)
	LinePosition screen_;
	//左上とサイズ(HD:1280x720)
	LeftAndSize viewport_;

	//カメラのマトリックス
	Matrix3x3 cameraMatrix_;
	//ビューマトリックス
	Matrix3x3 viewMatrix_;
	//正規化デバイス座標系
	Matrix3x3 orthoMatrix_;
	//ビューポートマトリックス
	Matrix3x3 viewportMatrix_;
	//vpVp
	Matrix3x3 vpVpMatrix_;

public:

	/// <summary>
	/// コンストラクター
	/// </summary>
	Camera(Affine center, Speed speed);
	~Camera();

	/// <summary>
	/// ラインの描画
	/// </summary>
	void Draw();

	/// <summary>
	/// カメラのスケール
	/// </summary>
	/// <param name="keys"></param>
	void Scale(char* keys);

	/// <summary>
	/// カメラの回転
	/// </summary>
	/// <param name="keys"></param>
	void Rotation(char* keys);

	/// <summary>
	/// カメラの移動
	/// </summary>
	/// <param name="keys"></param>
	void Translate(char* keys);

	/// <summary>
	/// カメラの移動
	/// </summary>
	void Move(char* keys);

	/// <summary>
	/// カメラの処理をまとめた
	/// </summary>
	void Update(char* keys);

	/// <summary>
	/// vpVpMatrixのゲッター
	/// </summary>
	/// <returns></returns>
	Matrix3x3 GetVpVpMatrix() { return vpVpMatrix_; };
};

