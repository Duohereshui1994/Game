#pragma once
#include <vector>
#include "Character.h"
#include "Bullet.h"

//玩家状态
enum class PlayerState {
	OnGround,		//在地面待机
	UnderGround,	//在地下待机
	Up,				//从土里钻出来
	Down			//钻进土里

};
class Player : public Character
{
private:
	int textureHandleLeft_;		//地面上面向左侧贴图
	int textureHandleRight_;	//地面下面向右侧贴图
	int textureHandleUnder_;	//地下待机贴图
	int textureHandleDown_;		//钻进土里贴图
	int textureHandleUp_;		//钻出土贴图
	int mousePosX;				//鼠标x坐标保存位置
	int mousePosY;				//鼠标y坐标保存位置
	Vector2 mousePos;			//用向量保存上面两个鼠标位置

	float downFrame_;			//钻进土里动画的帧数序号
	float upFrame_;				//钻出土动画的帧数序号

	Vector2 UnderPos;			//地下待机位置
	Vector2 UpPos;				//钻出土位置 也是地面上的待机位置

	float attackCD_;			//攻击冷却时间计时器单位（秒） 改变攻速不在这里改 在player.cpp

	PlayerState state_ = PlayerState::OnGround; //玩家初始状态为地面上待机

	//子弹数组
	std::vector<Bullet> bullets_;
public:

	Player();
	~Player();
	//初始化 重写
	void Initialize() override;
	//更新
	void Update(char keys[], char preKeys[]);
	//描画 重写
	void Draw() override;
	//攻击
	void Attack();
	//地面地下状态切换
	void SwithGround(char keys[], char preKeys[]);
};

