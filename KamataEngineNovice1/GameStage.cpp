#include "GameStage.h"

GameStage::GameStage()
{
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

	bg_ = new Background();
	bg_->Initialize();

	filter_ = new Filter();
	filter_->Initialize();

	emotion_ = new Emotion();
	emotion_->Initialize();
}

void GameStage::Update(char keys[], char preKeys[])
{
	camera_->Update(keys);
	player_->Update(keys, preKeys);
	player_->Update(keys, preKeys, camera_);
	bg_->Update(camera_);
	filter_->Update(camera_);
	emotion_->Update(player_, camera_);
}

void GameStage::Draw()
{
	bg_->Draw();
	camera_->Draw();
	player_->Draw();
	emotion_->Draw();
	filter_->Draw();
}
