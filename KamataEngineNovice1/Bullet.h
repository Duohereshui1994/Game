#pragma once
#include "MathFunc.h"
#include "Camera.h"

class Bullet
{
private:
	//速度
	float speed_;
	//==================camera=================================

	//プレイヤーのローカル座標
	Vertex local_;

	//プレイヤー 拡縮・回転・移動
	Affine affine_;

	//スクリーン座標系に変化に使う
	Vertex screen_;

	//ワールドマトリックス
	Matrix3x3 worldMatrix_;
	//wvpVp
	Matrix3x3 wvpVpMatrix_;
	//=========================================================


	//子弹的旋转
	//float rotate = 0.0f;

	//フラグ
	bool isShot_;

	//画像
	int textureBullet_;

	//目标位置的保存
	Vector2 targetPos_;

	//数学函数的指针
	MathFunc* math_ = nullptr;
	MathFunc::Object obj_;

public:
	Bullet();
	~Bullet();

	//初期化
	void Initialize();

	//更新
	void Update(const Vector2& target, const Vector2& playerPos, Camera* camera);

    //射出
	void Shoot(const Vector2& target, const Vector2& playerPos);

	//描画
	void Draw();
	void DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle);

	//位置取得
	Vector2 GetPos() {
		Vector2 pos;
		pos = affine_.translate;
		return pos;
	}

	//位置設定
	Vector2 SetPos(Vector2 pos) {
		this->affine_.translate = pos;
		return affine_.translate;
	}

	//幅取得
	float GetWidth() { return obj_.width; }

	//高さ取得
	float GetHeight() { return obj_.height;}

	//フラグ取得
	bool GetIsShot()const { return isShot_; }

	//フラグ設定
	bool SetIsShot(bool isShot) {
		this->isShot_ = isShot;
		return isShot_;
	}

	//设定目标位置
	void SetTargetPos(const Vector2& targetPos) {
		targetPos_ = targetPos;
	}

	Vector2 GetTargetPos() {return targetPos_; }
};

