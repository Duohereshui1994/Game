#include <Novice.h>
#include "Enemy.h"
#include "TitleStage.h"
#include "GameStage.h"

const char kWindowTitle[] = "6004_Game";


GameStage* gamestage_ = nullptr;
TitleStage* titleStage_ = nullptr;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	gamestage_ = new GameStage();

	gamestage_->Initialize();

	titleStage_ = new TitleStage();

	titleStage_->Initialize();

	//Enemy
	EnemyManager::LoadRes();//将本地贴图预先载入并保存
	Enemy* enemyObj = EnemyManager::AcquireEnemy({ 1280 / 2.f,720 / 2.f }, 0, Enemy::tSnake);//生成一个敌人
	enemyObj->PushUpdate();//将敌人推入主循环


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		/// 
		//titleStage_->Update();

		gamestage_->Update(keys, preKeys);
		//EnemyManager::Update(keys, preKeys);//敌人主循环

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		//titleStage_->Draw();

		gamestage_->Draw();
		//EnemyManager::Draw();//敌人主循环的绘画函数

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete gamestage_;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
