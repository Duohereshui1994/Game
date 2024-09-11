#include "GameStage.h"

GameStage::GameStage()
{
	EnemyManager::LoadRes();//预先载入敌人需要的贴图
	for (int i = 0; i < 100; i++)//预先生成一些敌人类，防止大量生成时候的卡顿
		EnemyManager::_idlePool.push(new Enemy());
}

GameStage::~GameStage()
{
	delete player_;
}

void GameStage::Initialize()
{
	player_ = new Player();
	player_->Initialize();

	//Affine cameraCenter{ {1.0f,1.0f},0,{0,0} };//原点
	Affine cameraCenter{ {1.0f,1.0f},0,{640,360} };//原点
	Speed cameraSpeed{ 1.0f / 60.0f,1.0f / 60.0f,5 };//拡縮や回転や移動のスピード
	camera_ = new Camera(cameraCenter, cameraSpeed);//インスタンス

	EnemyManager::ClearAllEnemy();	//清除所有敌人
	Score::Initialize();			//重置分数

	bg_ = new Background();
	bg_->Initialize();
}

void GameStage::Update(char keys[], char preKeys[])
{
	EnemyManager::BornEnemy(camera_, Score::GetScore(), 0);//生成敌人(相机，分数，小伙伴人数)

	camera_->Update(keys);
	EnemyManager::Update(keys, preKeys);
	player_->Update(keys, preKeys, camera_);
	bg_->Update(camera_);
	ParticleManager::Update();

	IsCollision();			//碰撞检测
	Score::Update(0);		//分数计算（要传入当前小伙伴数量）
}

void GameStage::Draw()
{
	bg_->Draw();
	camera_->Draw();
	EnemyManager::Draw();
	player_->Draw();
	ParticleManager::Draw();

	Score::Draw();
}

void GameStage::IsCollision()
{
	//敌人和子弹
	for (Enemy* enemy : EnemyManager::_updatePool) {
		if (!enemy->Get_isDead()) {
			for (auto& bullet : player_->bullets_) {
				float length = (enemy->GetTranslate() - bullet.GetPos()).Length();
				if (length < enemy->GetRadian() + bullet.GetWidth() / 2.f) {
					ParticleManager::ADD_Particle(camera_, bullet.GetPos(), Emitter::bulletHurt);
					Score::AddScore(enemy);
					bullet.Initialize();
					enemy->Set_isDead(true);
				}
			}
		}
	}
	//敌人和玩家
	for (Enemy* it : EnemyManager::_updatePool) {
		float length = (it->GetTranslate() - player_->GetTranslate()).Length();
		if (length < it->GetRadian() + player_->GetRadian()) {
			if (it->Get_type() == Enemy::tPlayer) {
				//和小伙伴触碰
				//可以使用下面提供的这个坐标去生成一个小伙伴，这样就可以无缝衔接上了
				Vector2 friendPos = it->GetTranslate();
				EnemyManager::ReleaseEnemy(it);
			}
			else {
				//和敌人触碰
				//需要提供一个小伙伴的坐标，这样我才知道要去哪里抓小伙伴
			}
		}
	}
}
