#pragma once
#include "MathFunc.h"
#include "Camera.h"
#include "Player.h"

class Player;

class Emotion
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

	int textureEmotion_;

	int stateTextureRange_;

	MathFunc* math_ = nullptr;	//数学函数类的指针

	MathFunc::Object obj_;		//数学函数类里 对象结构体的实例
public:
	Emotion();

	~Emotion();

	//初始化
	void Initialize();

	//更新
	void Update(Player* player, Camera* camera);

	//描画
	void Draw();
	void DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle);
};

