#include "HitCheck.h"

bool HitCheck_FromCenter::operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1)
{
	// 中心を起点にして当たり判定の実施
	if (pos2.x >= pos1.x - (size1.x / 2) && pos2.x <= pos1.x + (size1.x / 2)
		&& pos2.y >= pos1.y - (size1.y / 2) && pos2.y <= pos1.y + (size1.y / 2))
	{
		return true;
	}
	// 当たっていない
	return false;
}

bool HitCheck_FromLeftUp::operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1)
{
	// 左上を起点にして当たり判定の実施
	if (pos2.x >= pos1.x && pos2.y >= pos1.y &&
		pos2.x <= pos1.x + size1.x && pos2.y <= pos1.y + size1.y)
	{
		return true;
	}
	// 当たっていない
	return false;
}
