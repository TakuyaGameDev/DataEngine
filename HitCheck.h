#pragma once
#include "Geometory.h"

// ���S���N�_�ɂ��������蔻��
// param@ pos1: �����蔻��Ώ۵�޼ު�Ă��߼޼��
//        pos2: �����蔻��Ώۂ��߼޼�� 
//        size1: �����蔻��Ώ�(pos1)�̵�޼ު�Ļ���
struct HitCheck_FromCenter
{
	bool operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1);
};
// ���[���N�_�ɂ��������蔻��
// param@ pos1: �����蔻��Ώ۵�޼ު�Ă��߼޼��
//        pos2: �����蔻��Ώۂ��߼޼�� 
//        size1: �����蔻��Ώ�(pos1)�̵�޼ު�Ļ���
struct HitCheck_FromLeftUp
{
	bool operator()(const Vector2I& pos1, const Vector2I& pos2, const Vector2I size1);
};