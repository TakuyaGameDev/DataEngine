#pragma once
#include "Geometory.h"

// 中心を起点にした当たり判定
// param@ pos1: 当たり判定対象ｵﾌﾞｼﾞｪｸﾄのﾎﾟｼﾞｼｮﾝ
//        pos2: 当たり判定対象のﾎﾟｼﾞｼｮﾝ 
//        size1: 当たり判定対象(pos1)のｵﾌﾞｼﾞｪｸﾄｻｲｽﾞ
struct HitCheck_FromCenter
{
	bool operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1);
};
// 左端を起点にした当たり判定
// param@ pos1: 当たり判定対象ｵﾌﾞｼﾞｪｸﾄのﾎﾟｼﾞｼｮﾝ
//        pos2: 当たり判定対象のﾎﾟｼﾞｼｮﾝ 
//        size1: 当たり判定対象(pos1)のｵﾌﾞｼﾞｪｸﾄｻｲｽﾞ
struct HitCheck_FromLeftUp
{
	bool operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1);
};