#include "GameStage.h"

GameStage::GameStage()
{
	EnemyManager::LoadRes();//预先载入敌人需要的贴图
	ParticleManager::LoadRes();
	for (int i = 0; i < 100; i++)//预先生成一些敌人类，防止大量生成时候的卡顿
		EnemyManager::_idlePool.push(new Enemy());
}

GameStage::~GameStage()
{
	delete player_;
	delete bg_;
	delete filter_;
	delete emotion_;
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

	filter_ = new Filter();
	filter_->Initialize();

	emotion_ = new Emotion();
	emotion_->Initialize();

	grid_ = new Grid();
	grid_->Initialize();
}

void GameStage::Update(char keys[], char preKeys[])
{
	//scene 切换 test
	if (keys[DIK_R] && !preKeys[DIK_R]) {
		finished_ = true;
	}

	EnemyManager::BornEnemy(camera_, Score::GetScore(), player_->GetFriendCount());//生成敌人(相机，分数，小伙伴人数)

	camera_->Update(keys);
	player_->Update(keys, preKeys);
	EnemyManager::Update(keys, preKeys);
	player_->Update(keys, preKeys, camera_);
	bg_->Update(camera_);
	filter_->Update(player_, camera_, player_->GetFriendCount());
	emotion_->Update(player_, camera_);
	ParticleManager::Update();

	IsCollision();			//碰撞检测
	Score::Update(player_->GetFriendCount());		//分数计算（要传入当前小伙伴数量）

#ifdef _DEBUG
	//Particle test
	if (keys[DIK_1] && !preKeys[DIK_1]) {
		ParticleManager::ADD_Particle(camera_, player_->GetTranslate(), Emitter::happy);
	}
	if (keys[DIK_2] && !preKeys[DIK_2]) {
		ParticleManager::ADD_Particle(camera_, player_->GetTranslate(), Emitter::unHappy);
	}
#endif // _DEBUG
}

void GameStage::Draw()
{
	Novice::DrawBox(-50, -50, 1280 + 100, 720 + 100, 0, BLACK, kFillModeSolid);//最下面的黑色背景，防止穿帮

	bg_->Draw();
	//camera_->Draw();
	EnemyManager::Draw();
	player_->Draw();
	ParticleManager::Draw();
	filter_->Draw();
	grid_->Draw(0, 0);

	ParticleManager::PreDraw();
	Score::Draw();
}

void GameStage::IsCollision()
{
	//敌人和子弹
	for (Enemy* enemy : EnemyManager::_updatePool) {
		if (!enemy->Get_isDead() && !enemy->Get_isGetPlayer()) {
			for (auto& bullet : player_->bullets_) {
				float length = (enemy->GetTranslate() - bullet.GetPos()).Length();
				if (length < enemy->GetRadian() + bullet.GetWidth() / 2.f) {
					//特效
					ParticleManager::ADD_Particle(camera_, bullet.GetPos(), Emitter::bulletHurt);
					Vector2 enemyPos = enemy->GetTranslate();
					//地面的情况要将特效收缩到屏幕内
					if (player_->GetState() == PlayerState::OnGround) {
						enemyPos.x = std::clamp(enemyPos.x, 50.f, 1280.f - 50.f);
						enemyPos.y = std::clamp(enemyPos.y, 50.f, 720.f - 50.f);
					}
					if (enemy->Get_type() == Enemy::tPlayer)
						ParticleManager::ADD_Particle(camera_, enemyPos, Emitter::minusScore);
					else {
						if (enemy->GetTranslate().x > 0 && enemy->GetTranslate().x < 1280
							&& enemy->GetTranslate().y>0 && enemy->GetTranslate().y < 720)
							ParticleManager::ADD_Particle(camera_, enemyPos, Emitter::plusScore);
						else
							ParticleManager::ADD_Particle(camera_, enemyPos, Emitter::plusScore_long);
					}
					//分数(判断是否远距离击杀)
					if (enemy->GetTranslate().x > 0 && enemy->GetTranslate().x < 1280
						&& enemy->GetTranslate().y>0 && enemy->GetTranslate().y < 720)
						Score::AddScore(enemy, false);
					else
						Score::AddScore(enemy, true);
					//回收子弹和敌人
					bullet.Initialize();
					enemy->Set_isDead(true);
				}
			}
		}
	}
	//敌人和玩家
	for (Enemy* it : EnemyManager::_updatePool) {
		if (!it->Get_isDead() && !it->Get_isGetPlayer()) {
			Vector2 playerUpPos = { 640.0f, 220.0f };//因为玩家会上下运动，所以直接使用地面的坐标
			float length = (it->GetTranslate() - playerUpPos).Length();
			if (length < it->GetRadian() + player_->GetRadius()) {
				if (it->Get_type() == Enemy::tPlayer) {
					//和小伙伴触碰
					player_->OnFriendCollide(camera_);
					it->Set_isGetPlayer(true);
					EnemyManager::ReleaseEnemy(it);
				}
				else {
					//和敌人触碰
					it->Set_isGetPlayer(true);
					player_->OnEnenyCollide(camera_);
				}
			}
		}
	}
}
