#pragma once
#include "Enemy.h"
#include "Novice.h"
#include <vector>

class Score
{
private:
	inline static int _score = 0;			//分数
	inline static int _highScore = 0;		//最高分数
	inline static int _magnification = 1;	//倍率
	inline static int _combo = 0;	//combo

	inline static int _sprite = 0;
	inline static int _spCombo = 0;
	inline static int _spComboNums = 0;
	inline static Vector2 scorePos = { 530,658 };	//分数显示的坐标
	inline static Vector2 _comboPos = { 1280.f / 2.f - 100 ,600 };

	static void DrawScore(Vector2 pos);
	static void DrawCombo();
public:
	static void Initialize();				//初始化
	static void Update();					//更新方法
	static void Draw();						//绘画方法

	static void AddScore(Enemy* enemy, bool longKill);	//增加分数
	static void GameOverScore();						//游戏结束，分数计算
	static void RefreshMagnification();					//刷新倍率(不使用了)
	static void AddCombo(Enemy* enemy);
	static void ClearMagnification() { _magnification = 1; };
	static void ClearComboe() { _combo = 0; };

	static const int& GetScore() { return _score; }
	static const int& GetHighScore() { return _highScore; }
};

