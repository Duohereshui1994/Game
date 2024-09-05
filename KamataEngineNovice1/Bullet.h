#pragma once
#include "MathFunc.h"

class Bullet
{
private:
	//座標
	Vector2 pos_;
	//速度
	float speed_;
	//弾の幅
	float width_;
	//弾の高さ
	float height_;
	//フラグ
	bool isShot_;
	//画像
	int textureBullet_;


	MathFunc* math_ = nullptr;

public:
	Bullet();
	~Bullet();
	//初期化
	void Initialize();
	//更新
	void Update(const Vector2& playerPos, const Vector2& mousePos);
	void Shoot(const Vector2& playerPos, const Vector2& mousePos);
	//描画
	void Draw();
	//位置取得
	Vector2 GetPos() { return pos_; }
	//位置設定
	Vector2 SetPos(Vector2 pos) {
		this->pos_ = pos;
		return pos_;
	}
	//幅取得
	float GetWidth() { return width_; }
	//高さ取得
	float GetHeight() { return height_; }
	//フラグ取得
	bool GetIsShot()const { return isShot_; }
	//フラグ設定
	bool SetIsShot(bool isShot) {
		this->isShot_ = isShot;
		return isShot_;
	}
};

