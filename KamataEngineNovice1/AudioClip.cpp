#include "AudioClip.h"
#include "Novice.h"

AudioClip::AudioClip()
{
	audioTitle_ = Novice::LoadAudio("./RS/BGM/title_BGM.mp3");
	audioEffectButton_ = Novice::LoadAudio("./RS/BGM/effect_button.mp3");
	audioGameBgm_ = Novice::LoadAudio("./RS/BGM/main_BGM.mp3");

	audioHitEnemy = Novice::LoadAudio("./RS/BGM/hit_enemy.mp3");
	audioHitFriend = Novice::LoadAudio("./RS/BGM/hit_friends.mp3");
	audioJoinFriend = Novice::LoadAudio("./RS/BGM/friends_join.mp3");
	audioDeadFrend = Novice::LoadAudio("./RS/BGM/friends_dead.mp3");
	audioScore = Novice::LoadAudio("./RS/BGM/score.mp3");
	audioMushroom = Novice::LoadAudio("./RS/BGM/mushroom.mp3");
	audioStone = Novice::LoadAudio("./RS/BGM/stone.mp3");
	audioUpAndDown = Novice::LoadAudio("./RS/BGM/space_up_down.mp3");
}
