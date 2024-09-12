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

	inline static int _sprite = 0;
	inline static Vector2 scorePos = { 530,658 };	//分数显示的坐标
public:
	static void Initialize();				//初始化
	static void Update();					//更新方法
	static void Draw();						//绘画方法

	static void AddScore(Enemy* enemy, bool longKill);	//增加分数
	static void GameOverScore();						//游戏结束，分数计算
	static void ScoreDraw(Vector2 pos);
	static void AddMagnification(Enemy* enemy);
	static void ClearMagnification();

	static const int& GetScore() { return _score; }
	static const int& GetHighScore() { return _highScore; }
};

