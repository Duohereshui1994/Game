#include "Grid.h"
#include "Novice.h"

Grid::Grid()
{
	textureGrid_ = Novice::LoadTexture("./RS/UI/UI_Grid.png");
}

Grid::~Grid()
{
}

void Grid::Initialize()
{
}

void Grid::Update()
{
}

void Grid::Draw()
{
	Novice::DrawSprite(0, 0, textureGrid_, 1, 1, 0.0f, WHITE);
}
