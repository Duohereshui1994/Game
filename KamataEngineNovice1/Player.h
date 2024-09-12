#pragma once
#include <vector>
#include "Character.h"
#include "Bullet.h"
#include "Camera.h"

//玩家状态
enum class PlayerState {
	OnGround,		//在地面待机
	UnderGround,	//在地下待机
	Up,				//从土里钻出来
	Down			//钻进土里

};

enum class EmotionState {
	Null,
	Happy,
	General,
	Sad,
};

class Player : public Character
{
private:
	int textureHandleLeft_;		//地面上面向左侧贴图
	int textureHandleRight_;	//地面下面向右侧贴图

	int textureHandleLeftPure_;
	int textureHandleRightPure_;

	int textureHandleLeftControl_;
	int textureHandleRightControl_;

	int textureHandleUnder_;	//地下待机贴图
	int textureHandleDown_;		//钻进土里贴图
	int textureHandleUp_;		//钻出土贴图
	int textureHandleHand1_;		//手贴图1
	int textureHandleHand2_;		//手贴图1

	int mousePosX;				//鼠标x坐标保存位置
	int mousePosY;				//鼠标y坐标保存位置
	Vector2 mousePos;			//用向量保存上面两个鼠标位置
	Vector2 mousePosWorld;

	float downFrame_;			//钻进土里动画的帧数序号
	float upFrame_;				//钻出土动画的帧数序号

	int emotionValue_;				//情绪值
	int emotionRecover_;			//情绪回复倍率

	int currentFriendIndex = 13;  // 当前小伙伴数组下标
	int friendCount;			// 友方数量
	float radiusParam_;			// 半径参数

	float handRotate_;

	//==================camera=================================

	//プレイヤーのローカル座標
	Vertex local_;

	Vertex localHand_;

	Vertex localFriends_[14];

	//プレイヤー 拡縮・回転・移動
	Affine affine_;

	Affine affineHand_;

	Affine affineFriends_[14];

	//スクリーン座標系に変化に使う
	Vertex screen_;

	Vertex screenHand_;

	Vertex screenFriends_[14];

	//ワールドマトリックス
	Matrix3x3 worldMatrix_;

	Matrix3x3 worldMatrixHand_;

	Matrix3x3 worldMatrixFriends_[14];

	//wvpVp
	Matrix3x3 wvpVpMatrix_;

	Matrix3x3 wvpVpMatrixHand_;

	Matrix3x3 wvpVpMatrixFriends_[14];
	//=========================================================


	Vector2 UnderPos;			//地下待机位置（中心）
	Vector2 UpPos;				//钻出土位置 也是地面上的待机位置 (中心)

	float attackCD_;			//攻击冷却时间计时器单位（秒） 改变攻速不在这里改 在player.cpp

	PlayerState state_ = PlayerState::OnGround; //玩家初始状态为地面上待机
	EmotionState emotionState_ = EmotionState::Happy; //玩家初始情绪为开心

	Vector2 UpCameraScale = Vector2(1.0f, 1.0f);//玩家在地面上摄像机的倍率
	Vector2 DownCameraScale = Vector2(1.5f, 1.5f);//玩家在地下摄像机的倍率

	typedef struct {
		Vector2 pos_;
		Vector2 underPos_;
		unsigned int color;
		bool isAlive_;
	}Friends;




public:
	//子弹数组
	int _bullet_now = 0;		//当前子弹数
	int _bullet_max = 0;		//当前子弹最大上限
	int _bullet_targetMax = 5;	//这个值决定最初决定子弹的上限
	std::vector<Bullet> bullets_;
	//友方数组 最多14个友方
	Friends friends_[14];

	Player();

	~Player();
	//初始化 重写
	void Initialize() override;
	//更新
	void Update(char keys[], char preKeys[]);
	void Update(char keys[], char preKeys[], Camera* camera);
	//描画 重写
	void Draw() override;
	void DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle);

	void SetFriendArray();

	//攻击
	void Attack(char keys[], char preKeys[], Camera* camera);
	//地面地下状态切换
	void SwithGround(char keys[], char preKeys[], Camera* camera);
	//计算手的旋转
	float HandCal();

	Vector2 GetTranslate() { return affine_.translate; }							//获取中心点
	void SetTranslate(const Vector2& translate) { affine_.translate = translate; }	//设置中心点
	float GetRadius() { return obj_.width / 2.0f * radiusParam_; }					//获取图片宽度的一半当半径

	void OnEnenyCollide(Camera* camera);											//玩家与敌人碰撞运行
	void OnFriendCollide(Camera* camera);											//玩家与友方碰撞运行

	PlayerState GetState() { return state_; }	//获取玩家状态
	EmotionState GetEmotion() { return emotionState_; }//获取玩家情绪状态
	int GetEmotionValue() { return emotionValue_; }//获取玩家情绪值

	void EmotionUpdate();

	float GetUpFrame() { return upFrame_; }
	float GetDownFrame() { return downFrame_; }

	int GetFriendCount() { return friendCount; }

};

