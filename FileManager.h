#pragma once
#include <string>
#include <vector>

#define lpFileManager FileManager::getInstance()

class ColliderBoxManager;
// ̧�ُ������݂�ǂݍ��݁A̧�ّ���֌W���܂Ƃ߂��׽
// �ݸ���ݲݽ�ݽ
class FileManager
{
public:
	static FileManager& getInstance(void)
	{
		return *instance_;
	}

	static void Create(void);
	void Destroy(void);
	// �w���ިڸ�ؖ���肻���ިڸ�ؓ��ɑ��݂���̧�ق��������āA�������ʂ��i�[����
	void SearchFileFromDirectory(const std::string& directoryName);
	// �w�薼��̧�ق��������Ďw�薼��̧�ق��L���true��Ԃ�
	bool IsExistFile(const std::string& dataName,const std::string& objName,const std::string& animName);
	// �ް���̧�ق̓ǂݍ���
	void ReadDataFile(const std::string& pathName);

	// �ް���̧�ُ�������
	void WriteImageDataFile(const std::string& fileName,const std::vector<std::string>& strVec);
	// ��`�ް���̧�ُ�������
	void WriteRectDataFile(const std::string& fileName, const ColliderBoxManager& boxManager);
	// ̧�ٖ���ؽĂ̒��g�̸ر
	void ClearFileNameList(void);

	// �����������ʂ�̧�ٖ���ؽĂ̎擾
	const std::vector<std::string>& GetFileNameList(void) const
	{
		return fileNameList_;
	}
	// ��Ұ��ݖ���ؽĂ̎擾
	const std::vector<std::string>& GetAnimationNameList(void)const
	{
		return animationNameList_;
	}

private:
	FileManager() = default;
	~FileManager();
	// ��߰�֎~
	void operator=(const FileManager&) = delete;
	static FileManager* instance_;
	// �ިڸ�ؓ����ɑ��݂���̧�ٖ��B
	std::vector<std::string> fileNameList_;
	// ��޼ު�Ė��ɑ΂����Ұ��ݖ���ؽ�
	std::vector<std::string> animationNameList_;
};

