#define BULLET_WIDTH 45.0f
#define BULLET_HEIGHT 45.0f
#define BULLET_SPEED 5.0f			//子弹的速度
#define BULLET_ROTATE_TIMESCALE 1.0f//子弹旋转时的时间倍率 每帧加 1.0f / 60.0f * BULLET_ROTATE_TIMESCALE * (float)M_PI;
#include "Bullet.h"
#include "Novice.h"

//如果用矩阵的计算 所有的obj.center都没用了

Bullet::Bullet()
{
	speed_ = BULLET_SPEED;
	textureBullet1_ = Novice::LoadTexture("./RS/Bullet/bullet_mushroom.png");
	textureBullet2_ = Novice::LoadTexture("./RS/Bullet/bullet_stone.png");

	// 初始化 MathFunc 对象
	math_ = new MathFunc();


	// 初始化 Mono 结构体成员
	obj_.position = { obj_.center.x - obj_.width,obj_.center.y + obj_.height };
	obj_.width = BULLET_WIDTH;
	obj_.height = BULLET_HEIGHT;
	obj_.center = Vector2(0.0f, 0.0f);
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;

	//プレイヤーのローカル座標
	local_ = {
		{ -obj_.width / 2.0f,+obj_.height / 2.0f},
		{ +obj_.width / 2.0f,+obj_.height / 2.0f},
		{ -obj_.width / 2.0f,-obj_.height / 2.0f},
		{ +obj_.width / 2.0f,-obj_.height / 2.0f},
	};

	//プレイヤー 拡縮・回転・移動
	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };

	//スクリーン座標系に変化に使う
	screen_ = {};

	//ワールドマトリックス
	worldMatrix_ = {};
	//wvpVp
	wvpVpMatrix_ = {};


	targetPos_ = {};
	isShot_ = false;
}

Bullet::~Bullet()
{
	delete math_;
}

void Bullet::Initialize()
{
	obj_.center = Vector2(0.0f, 0.0f);
	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	isShot_ = false;
}

void Bullet::Update(const Vector2& target, const Vector2& playerPos, Camera* camera) {

	if (bulletSwitch_ == true) {
		bulletState_ = BulletType::Mushroom;
	}
	else {
		bulletState_ = BulletType::Stone;
	}
	// 射击
	if (isShot_) {
		Shoot(target, playerPos);
	}

	if (obj_.rotate > 2.0f * (float)M_PI) {
		obj_.rotate = 0;
	}
	else {
		obj_.rotate += 1.0f / 60.0f * BULLET_ROTATE_TIMESCALE * (float)M_PI;
	}

	affine_.theta = obj_.rotate;

	worldMatrix_ = math_->MakeAffine(affine_);
	wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, camera->GetVpVpMatrix());
	screen_ = math_->TransformSprite(local_, wvpVpMatrix_);

}


void Bullet::Shoot(const Vector2& target, const Vector2& playerPos)
{
	targetPos_ = target;
	// 更新子弹的移动
	Vector2 dir = math_->Subtract(targetPos_, playerPos);
	affine_.translate = math_->Add(affine_.translate, math_->Multiply(speed_, math_->Normalize(dir)));

	// 可以加一个判断，确保子弹超出屏幕或者达到目标后停止
	if (affine_.translate.x > 1600 || affine_.translate.x < -320 || affine_.translate.y > 900 || affine_.translate.y < -180) {
		Initialize();
	}

}


void Bullet::Draw()
{
	if (isShot_)
	{
		if (bulletSwitch_) {
			DrawTexture(0, 0, (int)BULLET_WIDTH, (int)BULLET_HEIGHT, textureBullet1_);
		}
		else {
			DrawTexture(0, 0, (int)BULLET_WIDTH, (int)BULLET_HEIGHT, textureBullet2_);
		}
	}
}

void Bullet::DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle)
{
	Novice::DrawQuad(
		(int)screen_.leftTop.x, (int)screen_.leftTop.y,
		(int)screen_.rightTop.x, (int)screen_.rightTop.y,
		(int)screen_.leftBottom.x, (int)screen_.leftBottom.y,
		(int)screen_.rightBottom.x, (int)screen_.rightBottom.y,
		leftTopX, leftTopY, width, height, textureHandle, WHITE);
}
