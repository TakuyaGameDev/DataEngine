#include <DxLib.h>
#include "ColliderBox.h"

ColliderBox::ColliderBox(const Vector2I& pos, const Vector2I& size, const bool& typeFlag):
	pos_(pos),size_(size),typeFlag_(typeFlag)
{

}

ColliderBox::~ColliderBox()
{
}

void ColliderBox::Draw(void)
{
	// 矩形の色
	unsigned int color = 0xffffff;
	// 矩形ﾀｲﾌﾟで色の変更
	if (typeFlag_)
	{
		// 赤に(攻撃矩形の場合)
		color = 0xff0000;
	}
	else
	{
		// 緑に(ﾀﾞﾒｰｼﾞ矩形の場合)
		color = 0x00ff00;
	}
	// 矩形の描画
	DrawBox(pos_.x - (size_.x/2), pos_.y - (size_.y / 2),
		pos_.x + (size_.x/2), pos_.y + (size_.y / 2), color, false);
}
