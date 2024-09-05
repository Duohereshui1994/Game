#pragma once
#include <vector>
#include "Character.h"
#include "Bullet.h"
class Player : public Character
{
private:
	int textureHandle2_;
	int mousePosX;
	int mousePosY;
public:
	std::vector<Bullet> bullets_;
	Player();
	~Player();
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

