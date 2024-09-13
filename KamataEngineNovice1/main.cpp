#include <Novice.h>
#include "TitleStage.h"
#include "GameStage.h"
#include "ClearStage.h"
#include "AudioClip.h"

const char kWindowTitle[] = "6004_モグラの守り手";

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
AudioClip* audioClip_ = nullptr;

bool isGameOver = false;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleStage_->IsFinished()) {
			if (titleStage_->GetButtonType() == TitleButtonType::kStart) {
				scene = Scene::kGame;
				delete titleStage_;
				titleStage_ = nullptr;
				gamestage_ = new GameStage();
				gamestage_->Initialize();
			}
			else if (titleStage_->GetButtonType() == TitleButtonType::kExit) {
				isGameOver = true;
			}
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
			if (clearStage_->GetButtonType() == ClearButtonType::kRestart) {
				scene = Scene::kGame;
				delete clearStage_;
				clearStage_ = nullptr;
				gamestage_ = new GameStage();
				gamestage_->Initialize();

			}
			else if (clearStage_->GetButtonType() == ClearButtonType::kMenu) {
				scene = Scene::kTitle;
				delete clearStage_;
				clearStage_ = nullptr;
				titleStage_ = new TitleStage();
				titleStage_->Initialize();
			}
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

	//sound
	int voiceHandleTitle_ = -1;
	int voiceHandleGame_ = -1;




	isGameOver = false;

	//贴图
	int _spMouse = Novice::LoadTexture("./RS/UI/mouse.png");
	Novice::SetMouseCursorVisibility(false);//不显示鼠标图标

	gamestage_ = new GameStage();

	titleStage_ = new TitleStage();

	clearStage_ = new ClearStage();

	audioClip_ = new AudioClip();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleStage_->Initialize();

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
			Novice::StopAudio(voiceHandleGame_);
			if (Novice::IsPlayingAudio(voiceHandleTitle_) == 0 || voiceHandleTitle_ == -1) {
				voiceHandleTitle_ = Novice::PlayAudio(audioClip_->audioTitle_, true, 0.0f);
			}
			titleStage_->Update();
			break;
		case Scene::kGame:
			Novice::StopAudio(voiceHandleTitle_);
			if (Novice::IsPlayingAudio(voiceHandleGame_) == 0 || voiceHandleGame_ == -1) {
				voiceHandleGame_ = Novice::PlayAudio(audioClip_->audioGameBgm_, true, 0.3f);
			}
			gamestage_->Update(keys, preKeys);
			break;
		case Scene::kClear:
			Novice::StopAudio(voiceHandleTitle_);
			Novice::StopAudio(voiceHandleGame_);
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

		//Mouse图标
		int mouseX = 0, mouseY = 0;
		Novice::GetMousePosition(&mouseX, &mouseY);
		Novice::DrawSprite(mouseX - 32, mouseY - 32, _spMouse, 1, 1, 0, WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
		if (isGameOver) {
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
