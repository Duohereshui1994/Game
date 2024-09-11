#pragma once
#include "MathFunc.h"
#include "Camera.h"

class Background
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

	int textureBackground_;		//背景图星空
	int textureGround_;			//背景图 泥土地面
	int textureKusaDoro_;		//背景图表面草地和浮土

	float frameNum_;			//背景动画帧数
	float deltaTime_;			//时间间隔

	MathFunc* math_ = nullptr;	//数学函数类的指针

	MathFunc::Object obj_;		//数学函数类里 对象结构体的实例

public:
	Background();
	~Background();
	//初始化
	void Initialize();
	//更新
	void Update(Camera* camera);
	//描画
	void Draw();
	void DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle);
};

