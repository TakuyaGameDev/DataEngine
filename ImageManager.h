#pragma once
#include <map>
#include <string>
#include <vector>

#include "Geometory.h"

using VecInt = std::vector<int>;

#define lpImageManager ImageManager::getInstance()

// �摜��۰�ނ�摜����ق̎擾����ړI�Ƃ����׽
// �ݸ���ݲݽ�ݽ
class ImageManager
{
public:
	static ImageManager& getInstance(void)
	{
		static ImageManager sInstance_;
		return sInstance_;
	}
	// �摜�P��۰��
	void Load(std::string name);
	// �摜����۰��
	void Load(std::string name, Vector2I divcnt, Vector2I divsize);

	// �摜����َ擾
	// param@ name: �摜̧�ٖ�
	const VecInt& GetHandle(std::string name)
	{
		return handles_[name];
	}

private:
	ImageManager() = default;
	// ��߰�֎~
	void operator=(const ImageManager&) = delete;
	// ۰�ގ��ɉ摜����ق��i�[����ϐ�
	// �摜̧�ٖ��Ŏ擾���₷���悤��std::map<std::string>�ɂ��Ă���
	std::map<std::string, VecInt> handles_;
};

