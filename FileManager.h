#pragma once
#include <string>
#include <vector>

#define lpFileManager FileManager::getInstance()

class ColliderBoxManager;
// ﾌｧｲﾙ書き込みや読み込み、ﾌｧｲﾙ操作関係をまとめたｸﾗｽ
// ｼﾝｸﾞﾙﾄﾝｲﾝｽﾀﾝｽ
class FileManager
{
public:
	static FileManager& getInstance(void)
	{
		return *instance_;
	}

	static void Create(void);
	void Destroy(void);
	// 指定ﾃﾞｨﾚｸﾄﾘ名よりそのﾃﾞｨﾚｸﾄﾘ内に存在するﾌｧｲﾙを検索して、検索結果を格納する
	void SearchFileFromDirectory(const std::string& directoryName);
	// 指定名でﾌｧｲﾙを検索して指定名のﾌｧｲﾙが有ればtrueを返す
	bool IsExistFile(const std::string& dataName,const std::string& objName,const std::string& animName);
	// ﾃﾞｰﾀのﾌｧｲﾙの読み込み
	void ReadDataFile(const std::string& pathName);

	// ﾃﾞｰﾀのﾌｧｲﾙ書き込み
	void WriteImageDataFile(const std::string& fileName,const std::vector<std::string>& strVec);
	// 矩形ﾃﾞｰﾀのﾌｧｲﾙ書き込み
	void WriteRectDataFile(const std::string& fileName, const ColliderBoxManager& boxManager);
	// ﾌｧｲﾙ名のﾘｽﾄの中身のｸﾘｱ
	void ClearFileNameList(void);

	// 検索した結果のﾌｧｲﾙ名のﾘｽﾄの取得
	const std::vector<std::string>& GetFileNameList(void) const
	{
		return fileNameList_;
	}
	// ｱﾆﾒｰｼｮﾝ名のﾘｽﾄの取得
	const std::vector<std::string>& GetAnimationNameList(void)const
	{
		return animationNameList_;
	}

private:
	FileManager() = default;
	~FileManager();
	// ｺﾋﾟｰ禁止
	void operator=(const FileManager&) = delete;
	static FileManager* instance_;
	// ﾃﾞｨﾚｸﾄﾘ内部に存在するﾌｧｲﾙ名達
	std::vector<std::string> fileNameList_;
	// ｵﾌﾞｼﾞｪｸﾄ名に対するｱﾆﾒｰｼｮﾝ名のﾘｽﾄ
	std::vector<std::string> animationNameList_;
};

