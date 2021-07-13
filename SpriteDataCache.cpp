#include "SpriteDataCache.h"

SpriteDataCache* SpriteDataCache::instance_ = nullptr;

void SpriteDataCache::Create(void)
{
	if (!instance_)
	{
		instance_ = new SpriteDataCache();
	}
}

void SpriteDataCache::Destroy(void)
{
	if (instance_)
	{
		delete instance_;
	}
	instance_ = nullptr;
}

void SpriteDataCache::SetMaxFrame(const int& maxFrame)
{
	animationData_.maxFrame = maxFrame;
}

void SpriteDataCache::SetLoopFlag(const bool& flg)
{
	animationData_.loop = flg;
}

void SpriteDataCache::SetDuration(const float& duration)
{
	animationData_.duration = duration;
}

SpriteDataCache::~SpriteDataCache()
{
}
