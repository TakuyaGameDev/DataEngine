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
	// ��`�̐F
	unsigned int color = 0xffffff;
	// ��`���߂ŐF�̕ύX
	if (typeFlag_)
	{
		// �Ԃ�(�U����`�̏ꍇ)
		color = 0xff0000;
	}
	else
	{
		// �΂�(��Ұ�ދ�`�̏ꍇ)
		color = 0x00ff00;
	}
	// ��`�̕`��
	DrawBox(pos_.x - (size_.x/2), pos_.y - (size_.y / 2),
		pos_.x + (size_.x/2), pos_.y + (size_.y / 2), color, false);
}
