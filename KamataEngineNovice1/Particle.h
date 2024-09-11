#pragma once
#include <vector>
#include <queue>
#include <random>
#include <Novice.h>
#include "struct.h"
#include "Camera.h"
#include "MathFunc.h"

//粒子系统使用方法：
//这个系统写的太复杂了，所以就只要关注ParticleManager这个类即可
//重要的就是Update()和Draw()和PreDraw()，这3个方法要放到主循环中
//需要使用特效的时候就调用ADD_Particle()方法，要传入相机、出现的坐标、Emitter::的一个enum类型
//例：ADD_Particle(camera_,pos_,Emitter::emotion_happy);


class Particle
{
public:
	static enum TYPE
	{
		fireWorks,
		bulletHurt,
		minusScore,
		plusScore,
		emotion_happy,
		emotion_normal,
		emotion_sad,
	}_particleType;

	Vector2 _pos;
	Vector2 _vel;
	Vector2 _dir;
	Vector2 _acc;
	float _speed;
	float _gravity;
	Camera* _camera = nullptr;
	MathFunc* math_ = nullptr;	//数学函数类的指针

	TYPE _type;
	float _radius;			//物理判断半径
	float _angle;			//旋转
	Vector2 _spriteSize;	//贴图大小
	Vector2 _scale;			//尺寸
	int _sprite;
	unsigned int _color;
	int _alphaValue;

	int _currentTime;
	int _lifeTime;

	bool _isAudio;

	Particle(Camera* camera, Vector2 pos, TYPE type);
	~Particle() { delete _camera, delete math_; };
	void Initialize(Camera* camera, Vector2 pos, TYPE type);
	void Move();
	void Draw();
	void PreDraw();
	void ToDead();
	void PushUpdate();

	//工具
	unsigned int ColorInterpolation(unsigned int startColor, unsigned int targetColor, float percent);
	float EaseOutQuint(float t) { return 1.f - powf(1.f - t, 5.f); };
	float EaseInCirc(float t) { return 1 - sqrtf(1 - powf(t, 2)); };
};

class Emitter
{
public:
	static enum TYPE
	{
		fireWorks,
		bulletHurt,
		minusScore,
		plusScore,
		emotion_happy,
		emotion_normal,
		emotion_sad,
	}_particleType;

	Vector2 _pos;
	float _angle;
	unsigned int _color;
	float _width;//粒子が出る位置の範囲
	float _height;
	Vector2 _scale;
	Camera* _camera = nullptr;
	MathFunc* math_ = nullptr;	//数学函数类的指针

	TYPE _type;
	int _currentTime;
	int _lifeTime;
	bool _isLoop;//Trueの場合、ずっと生きている
	int _particleGetTime;//パーティクルを生まれる間時間(１フレーム１個の生まれる時間もちょっと早すぎる)
	int _particleCurrentTime;//時間を計算用
	int _particleSum;

	bool _isDead;

	Emitter(Camera* camera, Vector2 pos, TYPE type);
	~Emitter() { delete _camera, delete math_; };
	void Initialize(Camera* camera, Vector2 pos, TYPE type);
	void ParticleStart();
	void Draw();
	void PreDraw();
	void ToDead();
	void PushUpdate();
};

class ParticleManager
{
public:
	inline static std::vector<Particle*> _particleUpdatePool;
	inline static std::queue<Particle*> _particleIdiePool;
	inline static std::vector<Emitter*> _emitterUpadtePool;
	inline static std::queue<Emitter*> _emitterIdiePool;

	static void ParticleUpdate();
	static void ParticleUpdateShow();
	static Particle* AcquireParticle(Camera* camera, Vector2 startPos, Particle::TYPE type);
	static void ReleaseParticle(Particle* particle);

	static void EmitterUpdate();
	static void EmitterUpdateShow();
	static Emitter* AcquireEmitter(Camera* camera, Vector2 pos, Emitter::TYPE type);
	static void ReleaseEmitter(Emitter* emitter);

	//普通のパーティクル：位置、Type
	static void ADD_Particle(Camera* camera, Vector2 pos, Emitter::TYPE type);
	//更新用関数
	static void Update();
	//描画用関数
	static void Draw();
	static void PreDraw();//前景绘图
};
