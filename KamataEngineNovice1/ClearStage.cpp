#include "ClearStage.h"
#include "Novice.h"


ClearStage::ClearStage()
{
}

ClearStage::~ClearStage()
{
}

void ClearStage::Initialize()
{
}

void ClearStage::Update(char keys[], char preKeys[])
{
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		finished_ = true;
	}
}

void ClearStage::Draw()
{
}
