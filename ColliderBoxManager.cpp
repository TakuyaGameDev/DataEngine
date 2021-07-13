#include "ColliderBoxManager.h"
#include "ColliderBox.h"

ColliderBoxManager::ColliderBoxManager(int maxFrame)
{
	// ���������ڰѐ�����resize���Ă���
	colliderBoxList_.resize(maxFrame);
	// �n�_��0�ȊO�ł���΂Ȃ�ł������̂�
	// �Ƃ肠����-1����������������邱�Ƃ��Ȃ��̂�
	// ���̐����ŏ�����
	begPos_ = { -1,-1 };
}

ColliderBoxManager::~ColliderBoxManager()
{
}

void ColliderBoxManager::SetBegPos(const Vector2I& beg)
{
	// �n�_�̐ݒ�
	begPos_ = beg;
}

void ColliderBoxManager::AddColliderBox(const int& frame, const Vector2I& endPos,const bool& typeFlag)
{
	// ���ނ̐ݒ�
	auto size = Vector2I(endPos.x - begPos_.x, endPos.y - begPos_.y);
	// ���S���W�̐ݒ�
	auto center = Vector2I(begPos_.x + (size.x / 2), begPos_.y + (size.y / 2));
	// ��`�̐���
	auto tmpCol = std::make_shared<ColliderBox>(center, size,typeFlag);
	// ؽĂɒǉ�
	colliderBoxList_[frame].push_back(tmpCol);
	// �n�_��ؾ��
	begPos_ = { -1,-1 };
}

void ColliderBoxManager::Draw(const int& frame)
{
	// Add���ꂽ��`�̈ꊇ�`��
	for (auto& box : colliderBoxList_[frame])
	{
		box->Draw();
	}
}
