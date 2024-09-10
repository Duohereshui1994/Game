#include "Camera.h"
#include "Player.h"
#include <Novice.h>

Camera::Camera(Affine center, Speed speed)
{
	math_ = new MathFunc();
	affine_ = center;
	vertex_ = {
		 -SCREEN_WIDTH / 2.0f,
		  SCREEN_HEIGHT / 2.0f,
		  SCREEN_WIDTH / 2.0f,
		 -SCREEN_HEIGHT / 2.0f,
	};
	local_ = {
		{
		 { -10000,0 },
		 {  10000,0 },
		},
		{
		 { 0, -10000},
		 { 0,  10000},
		},
	};

	speed_ = speed;

	screen_ = {};

	viewport_ = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	cameraMatrix_ = {};

	viewMatrix_ = {};

	orthoMatrix_ = {};

	viewportMatrix_ = {};

	vpVpMatrix_ = {};
}

Camera::~Camera()
{
	delete math_;
}

void Camera::Draw()
{
	Novice::DrawLine((int)screen_.lateral.begin.x, (int)screen_.lateral.begin.y, (int)screen_.lateral.end.x, (int)screen_.lateral.end.y, RED);
	Novice::DrawLine((int)screen_.vert.begin.x, (int)screen_.vert.begin.y, (int)screen_.vert.end.x, (int)screen_.vert.end.y, GREEN);
}

void Camera::Scale(char* keys)
{
	if (keys[DIK_E] && !keys[DIK_LSHIFT]) {
		if (affine_.scale.x > 0.3f || affine_.scale.y > 0.3f) {
			affine_.scale.x -= speed_.scale;
			affine_.scale.y -= speed_.scale;
		}
	}
	if (keys[DIK_Q] && !keys[DIK_LSHIFT]) {
		affine_.scale.x += speed_.scale;
		affine_.scale.y += speed_.scale;
	}
}

void Camera::Rotation(char* keys)
{
	if (keys[DIK_C] && !keys[DIK_LSHIFT]) {
		affine_.theta -= speed_.rotate;
	}
	if (keys[DIK_Z] && !keys[DIK_LSHIFT]) {
		affine_.theta += speed_.rotate;
	}
}

void Camera::Translate(char* keys)
{
	if (keys[DIK_LEFT]) {
		affine_.translate.x += speed_.translate;
	}
	if (keys[DIK_RIGHT]) {
		affine_.translate.x -= speed_.translate;
	}
	if (keys[DIK_UP]) {
		affine_.translate.y -= speed_.translate;
	}
	if (keys[DIK_DOWN]) {
		affine_.translate.y += speed_.translate;
	}
}

void Camera::Move(char* keys)
{
	Scale(keys);
	Rotation(keys);
	Translate(keys);
}

void Camera::Update(char* keys)
{
	Move(keys);
	cameraMatrix_ = math_->MakeAffine(affine_);
	viewMatrix_ = math_->ViewMatrix(cameraMatrix_);
	orthoMatrix_ = math_->OrthoMatrix(vertex_);
	viewportMatrix_ = math_->ViewportMatrix(viewport_);
	vpVpMatrix_ = math_->MakeVpVpMatrix(viewMatrix_, orthoMatrix_, viewportMatrix_);
	screen_ = math_->TransformLine(local_, vpVpMatrix_);
}
