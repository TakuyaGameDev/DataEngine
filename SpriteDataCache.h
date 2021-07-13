#pragma once
#include <string>

#include "Geometory.h"

// ��Ұ��݂Ɋւ����ް�
struct AnimationData
{
	// ��Ұ��ݍő�l
	int maxFrame;
	// 1�ςɂ����鎞��
	float duration;
	// ���̱�Ұ��݂�ٰ�ߍĐ������̂����׸�
	bool loop;
};

#define lpSpriteDataCache SpriteDataCache::getInstance()

// ImageData�̊e���ް��i�[�׽(�ݸ���ݲݽ�ݽ)
class SpriteDataCache
{
public:
	static SpriteDataCache& getInstance(void)
	{
		return *instance_;
	}
	// ����
	static void Create(void);
	// �j��
	void Destroy(void);
	// �ő��ڰт̾��
	void SetMaxFrame(const int& maxFrame);
	// ٰ���׸ނ̾��
	void SetLoopFlag(const bool& flg);
	// duration�̾��
	void SetDuration(const float& duration);
	// ��Ұ����ް��̎擾
	const AnimationData& GetAnimationData(void)const
	{
		return animationData_;
	}

private:
	SpriteDataCache() = default;
	~SpriteDataCache();
	// ��߰�֎~
	void operator=(const SpriteDataCache&) = delete;
	static SpriteDataCache* instance_;
	// ��Ұ����ް�(��Ұ��ݖ��A�S���ŉ��ς���̂��Aٰ�߂���̂��A1�ςɂ����鎞��)
	AnimationData animationData_;
};

