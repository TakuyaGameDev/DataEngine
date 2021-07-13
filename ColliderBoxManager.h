#pragma once
#include <memory>
#include <vector>
#include "Geometory.h"

class ColliderBox;

// shared_ptr�Ő��߲����h��
using SharedCollider = std::shared_ptr<ColliderBox>;
// �ڰѐ������ςő��݂���̂�resize�\�Ȃ悤��vector���ɂ��Ă���
using ColliderVec = std::vector<SharedCollider>;

// �����蔻���`���܂Ƃ߂ĊǗ�����׽
// ������̕����܂Ƃ߂ď������₷��
class ColliderBoxManager
{
public:
	ColliderBoxManager(int maxFrame);
	~ColliderBoxManager();

	// �n�_���W�̾��
	// �د����ꂽ�羯Ă���
	void SetBegPos(const Vector2I& beg);

	// ��`�̎n�_�̎擾
	const Vector2I& GetBegPos(void)const
	{
		return begPos_;
	}

	// colliderBoxList_�ɒǉ�
	// param@ frame : ���݂��ڰ�(��) boxSize : ���̻���
	void AddColliderBox(const int& frame, const Vector2I& endPos,const bool& typeFlag);

	void Draw(const int& frame);
	// ��`ؽĂ̎擾
	const std::vector<ColliderVec>& GetColliderBoxList(void)const
	{
		return colliderBoxList_;
	}

private:
	// ����`�悵�n�߂�n�_���W
	Vector2I begPos_;
	// 1�ڰтɂ������������݂���̂ŉςɂ��Ă���
	std::vector<ColliderVec> colliderBoxList_;
};

