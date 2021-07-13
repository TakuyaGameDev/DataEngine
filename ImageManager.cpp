#include <DxLib.h>
#include "ImageManager.h"

void ImageManager::Load(std::string name)
{
	// handles��name�Ŏw�肳�ꂽ������̉摜����ق����݂��Ȃ��ꍇ
	// �d������۰�ނ��Ȃ��悤�ɶް�ނ�������
	if (handles_.count(name) <= 0)
	{
		// resize����
		handles_[name].resize(1);
		// �摜��۰��
		handles_[name][0] = LoadGraph(name.c_str());
	}
}

void ImageManager::Load(std::string name, Vector2I divcnt, Vector2I divsize)
{
	// handles��name�Ŏw�肳�ꂽ������̉摜����ق����݂��Ȃ��ꍇ
	// �d������۰�ނ��Ȃ��悤�ɶް�ނ�������
	if (handles_.count(name) <= 0)
	{
		// �摜����ق��摜����resize����
		handles_[name].resize(divcnt.x * divcnt.y);
		// �摜�̕���۰��
		LoadDivGraph(name.c_str(), divcnt.x * divcnt.y,
			divcnt.x, divcnt.y, divsize.x, divsize.y,
			&handles_[name][0]);
	}
}
