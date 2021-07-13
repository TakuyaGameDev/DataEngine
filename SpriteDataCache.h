#pragma once
#include <string>

#include "Geometory.h"

// ｱﾆﾒｰｼｮﾝに関するﾃﾞｰﾀ
struct AnimationData
{
	// ｱﾆﾒｰｼｮﾝ最大値
	int maxFrame;
	// 1ｺﾏにかかる時間
	float duration;
	// そのｱﾆﾒｰｼｮﾝがﾙｰﾌﾟ再生されるのかのﾌﾗｸﾞ
	bool loop;
};

#define lpSpriteDataCache SpriteDataCache::getInstance()

// ImageDataの各種ﾃﾞｰﾀ格納ｸﾗｽ(ｼﾝｸﾞﾙﾄﾝｲﾝｽﾀﾝｽ)
class SpriteDataCache
{
public:
	static SpriteDataCache& getInstance(void)
	{
		return *instance_;
	}
	// 生成
	static void Create(void);
	// 破棄
	void Destroy(void);
	// 最大ﾌﾚｰﾑのｾｯﾄ
	void SetMaxFrame(const int& maxFrame);
	// ﾙｰﾌﾟﾌﾗｸﾞのｾｯﾄ
	void SetLoopFlag(const bool& flg);
	// durationのｾｯﾄ
	void SetDuration(const float& duration);
	// ｱﾆﾒｰｼｮﾝﾃﾞｰﾀの取得
	const AnimationData& GetAnimationData(void)const
	{
		return animationData_;
	}

private:
	SpriteDataCache() = default;
	~SpriteDataCache();
	// ｺﾋﾟｰ禁止
	void operator=(const SpriteDataCache&) = delete;
	static SpriteDataCache* instance_;
	// ｱﾆﾒｰｼｮﾝﾃﾞｰﾀ(ｱﾆﾒｰｼｮﾝ名、全部で何ｺﾏあるのか、ﾙｰﾌﾟするのか、1ｺﾏにかかる時間)
	AnimationData animationData_;
};

