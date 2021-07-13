#pragma once
#include "Geometory.h"

// 矩形ｸﾗｽ
class ColliderBox
{
public:
	ColliderBox(const Vector2I& pos,const Vector2I& size,const bool& typeFlag);
	~ColliderBox();
	// 矩形のﾀｲﾌﾟ取得
	const bool& GetTypeFlag(void)const
	{
		return typeFlag_;
	}
	// 矩形の中心ﾎﾟｼﾞｼｮﾝの取得
	const Vector2I& GetPos(void)const
	{
		return pos_;
	}
	// 矩形ｻｲｽﾞの取得
	const Vector2I& GetSize(void)const
	{
		return size_;
	}
	// 自身の描画
	void Draw(void);

private:
	// 矩形の中心をさす
	const Vector2I pos_;
	// 矩形のｻｲｽﾞ
	const Vector2I size_;
	// Damage矩形かAttack矩形かの識別子
	// Damage -> false Attack -> true
	const bool typeFlag_;
};

