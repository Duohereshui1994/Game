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

	EnemyManager::ClearAllEnemy();//清除所有敌人
}

void GameStage::Update(char keys[], char preKeys[])
{
	EnemyManager::BornEnemy(camera_, 0, 0);//生成敌人(相机，分数，小伙伴人数)

	camera_->Update(keys);
	EnemyManager::Update(keys, preKeys);
	player_->Update(keys, preKeys, camera_);
}

void GameStage::Draw()
{
	camera_->Draw();
	EnemyManager::Draw();
	player_->Draw();
}
