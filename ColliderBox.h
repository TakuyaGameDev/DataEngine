#pragma once
#include "Geometory.h"

// ��`�׽
class ColliderBox
{
public:
	ColliderBox(const Vector2I& pos,const Vector2I& size,const bool& typeFlag);
	~ColliderBox();
	// ��`�����ߎ擾
	const bool& GetTypeFlag(void)const
	{
		return typeFlag_;
	}
	// ��`�̒��S�߼޼�݂̎擾
	const Vector2I& GetPos(void)const
	{
		return pos_;
	}
	// ��`���ނ̎擾
	const Vector2I& GetSize(void)const
	{
		return size_;
	}
	// ���g�̕`��
	void Draw(void);

private:
	// ��`�̒��S������
	const Vector2I pos_;
	// ��`�̻���
	const Vector2I size_;
	// Damage��`��Attack��`���̎��ʎq
	// Damage -> false Attack -> true
	const bool typeFlag_;
};

