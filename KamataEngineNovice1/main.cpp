#include <Novice.h>
#include "TitleStage.h"
#include "GameStage.h"
#include "ClearStage.h"

const char kWindowTitle[] = "6004_Game";

enum class Scene {
	kUnkonwn = 0,
	kTitle,
	kGame,
	kClear,
};
Scene scene = Scene::kUnkonwn;

GameStage* gamestage_ = nullptr;
TitleStage* titleStage_ = nullptr;
ClearStage* clearStage_ = nullptr;

void ChangeScene() {
	switch (scene) {
		case Scene::kTitle:
			if (titleStage_->IsFinished()) {
				scene = Scene::kGame;
				delete titleStage_;
				titleStage_ = nullptr;
				gamestage_ = new GameStage();
				gamestage_->Initialize();
			}
			break;
		case Scene::kGame:
			if (gamestage_->IsFinished()) {
				scene = Scene::kClear;
				delete gamestage_;
				gamestage_ = nullptr;
				clearStage_ = new ClearStage();
				clearStage_->Initialize();
			}
			break;
		case Scene::kClear:
			if (clearStage_->IsFinished()) {
				scene = Scene::kTitle;
				delete clearStage_;
				clearStage_ = nullptr;
				titleStage_ = new TitleStage();
				titleStage_->Initialize();
			}
			break;
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	gamestage_ = new GameStage();

	//gamestage_->Initialize();

	titleStage_ = new TitleStage();

	//titleStage_->Initialize();

	// 最初のシーンの初期化
	scene = Scene::kTitle;

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

		//gamestage_->Update(keys, preKeys);

		ChangeScene();

		//Update Scene
		switch (scene) {
			case Scene::kTitle:
				titleStage_->Update(keys, preKeys);
				break;
			case Scene::kGame:
				gamestage_->Update(keys, preKeys);
				break;
			case Scene::kClear:
				clearStage_->Update(keys, preKeys);
				break;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//titleStage_->Draw();

		//gamestage_->Draw();

		switch (scene) {
			case Scene::kTitle:
				titleStage_->Draw();
				break;
			case Scene::kGame:
				gamestage_->Draw();
				break;
			case Scene::kClear:
				clearStage_->Draw();
				break;
		}

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

	delete titleStage_;
	delete gamestage_;
	delete clearStage_;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
