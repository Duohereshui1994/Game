#pragma once
#include <vector>
#include "Character.h"
#include "Bullet.h"

enum class PlayerState {
	OnGround,
	UnderGround,
	Up,
	Down

};
class Player : public Character
{
private:
	int textureHandleLeft_;
	int textureHandleRight_;
	int textureHandleUnder_;
	int textureHandleDown_;
	int textureHandleUp_;
	int mousePosX;
	int mousePosY;
	Vector2 mousePos;

	float downFrame_;
	float upFrame_;

	bool isOnGround;
	Vector2 UnderPos;
	Vector2 UpPos;

	PlayerState state_ = PlayerState::OnGround;
public:
	std::vector<Bullet> bullets_;
	Player();
	~Player();
	void Initialize() override;
	void Update(char keys[], char preKeys[]);
	void Draw() override;
	void Attack();
	void SwithGround(char keys[], char preKeys[]);
};

