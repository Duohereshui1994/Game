#pragma once
class AudioClip
{
public:
	//bgm  1代表用了 2代表不知道在哪用
	int audioTitle_;//标题音乐	1
	int audioEffectButton_;//标题按键	1
	int audioGameBgm_;//游戏音乐	1

	//effect
	int audioHitEnemy;//打中敌人		1
	int audioHitFriend;//打中友军	1
	int audioJoinFriend;//伙伴加入	1
	int audioDeadFrend;//伙伴死亡	1
	int audioScore;//得分
	int audioMushroom;//蘑菇发射	1
	int audioStone;//石头发射	1
	int audioUpAndDown;//钻地	1
	int audioTama;//缺子弹	1

	AudioClip();


};

