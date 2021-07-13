#include "HitCheck.h"

bool HitCheck_FromCenter::operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1)
{
	// ���S���N�_�ɂ��ē����蔻��̎��{
	if (pos2.x >= pos1.x - (size1.x / 2) && pos2.x <= pos1.x + (size1.x / 2)
		&& pos2.y >= pos1.y - (size1.y / 2) && pos2.y <= pos1.y + (size1.y / 2))
	{
		return true;
	}
	// �������Ă��Ȃ�
	return false;
}

bool HitCheck_FromLeftUp::operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1)
{
	// ������N�_�ɂ��ē����蔻��̎��{
	if (pos2.x >= pos1.x && pos2.y >= pos1.y &&
		pos2.x <= pos1.x + size1.x && pos2.y <= pos1.y + size1.y)
	{
		return true;
	}
	// �������Ă��Ȃ�
	return false;
}
