#pragma once
//滤镜类
#include "MathFunc.h"
#include "Camera.h"

class Player;

class Filter
{
private:
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

	int textureFilter_;

	Vector2 upViewScale;
	Vector2 downViewScale;
	Vector2 upViewScale_target;

	MathFunc* math_ = nullptr;	//数学函数类的指针

	MathFunc::Object obj_;		//数学函数类里 对象结构体的实例

public:
	Filter();
	~Filter();
	//初始化
	void Initialize();
	//更新
	void Update(Player* player, Camera* camera, int friendSum);
	//描画
	void Draw();
	void DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle);
};

