#pragma once
#include "MathFunc.h"

class Bullet
{
private:
	//子弹中心座標
	Vector2 pos_;
	//速度
	float speed_;
	//弾の幅
	float width_;
	//弾の高さ
	float height_;

	//子弹的四个顶点世界坐标
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;

	//子弹的四个顶点屏幕坐标
	Vector2 leftTop_Screen;
	Vector2 rightTop_Screen;
	Vector2 leftBottom_Screen;
	Vector2 rightBottom_Screen;

	//子弹的旋转
	float rotate = 0.0f;

	//フラグ
	bool isShot_;

	//画像
	int textureBullet_;

	//目标位置的保存
	Vector2 targetPos_;

	//数学函数的指针
	MathFunc* math_ = nullptr;

public:
	Bullet();
	~Bullet();

	//初期化
	void Initialize();

	//更新
	void Update(const Vector2& playerPos, const Vector2& mousePos);

    //射出
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

	//设定目标位置
	void SetTargetPos(const Vector2& targetPos) {
		targetPos_ = targetPos;
	}
};

