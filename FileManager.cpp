#include <Windows.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "FileManager.h"
#include "ColliderBoxManager.h"
#include "ColliderBox.h"

FileManager* FileManager::instance_ = nullptr;

void FileManager::Create(void)
{
	if (!instance_)
	{
		// 生成
		instance_ = new FileManager();
	}
}

void FileManager::Destroy(void)
{
	if (instance_)
	{
		// 破棄
		delete instance_;
	}
	instance_ = nullptr;
}

void FileManager::SearchFileFromDirectory(const std::string& directoryName)
{
	if (fileNameList_.size() <= 0)
	{
		HANDLE hFind;
		WIN32_FIND_DATA win32fd;
		// 検索するﾌｧｲﾙに到達するまでのﾊﾟｽ
		std::string path = directoryName + "/";
		// 対象にするﾌｧｲﾙ名拡張子
		std::string extension = ".png";
		// 検索にかける名前
		// (*pngとは -> .pngとつくﾌｧｲﾙ名全てを検索かけるという事【ﾜｲﾙﾄﾞｶｰﾄﾞ】とも呼ぶ)
		std::string search_name = path + "*.png";

		hFind = FindFirstFile(search_name.c_str(), &win32fd);
		// ﾌｧｲﾙがなければ
		if (hFind == INVALID_HANDLE_VALUE) {
			throw std::runtime_error("file not found");
			return;
		}

		// 指定のディレクトリ以下のファイル名をファイルがなくなるまで取得する
		do {
			if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// ディレクトリの場合は何もしない
				continue;
			}
			else {
				fileNameList_.push_back(win32fd.cFileName);
			}
		} while (FindNextFile(hFind, &win32fd));
		// ﾌｧｲﾙを閉じる
		FindClose(hFind);
	}
}

bool FileManager::IsExistFile(const std::string& dataName,const std::string& objName,const std::string& animName)
{
	// 検索をかけるファイル名
	auto fileName = "DataFiles/Details/" + objName + "/" + dataName + "/" + animName;
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	// 検索にかける名前
	std::string search_name = fileName + ".dat";

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	// ﾌｧｲﾙが無ければfalseを返す
	if (hFind == INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		return false;
	}
	else
	{
		// ﾌｧｲﾙが有ればtrueを返す
		FindClose(hFind);
		return true;
	}
	return false;
}

void FileManager::ReadDataFile(const std::string& pathName)
{
	// 読み込むﾃﾞｰﾀﾌｧｲﾙ名
	std::string fileName = pathName + ".dat";
	// ﾃﾞｰﾀﾌｧｲﾙ
	std::ifstream file(fileName, std::ios::in | std::ios::binary);
	std::stringstream stringStream;
	std::string str1;

	do
	{
		// ｶﾝﾏ区切りで一行読む
		std::getline(file, str1,',');
		// ｱﾆﾒｰｼｮﾝ名のﾘｽﾄに格納していく
		animationNameList_.push_back(str1);
	} while (!file.eof());
	// ﾌｧｲﾙを閉じる
	file.close();
}

void FileManager::WriteImageDataFile(const std::string& fileName,const std::vector<std::string>& strVec)
{
	std::string dataFileName = "DataFiles/Details/" + fileName + ".dat";
	// 書き込むﾃﾞｰﾀﾌｧｲﾙ
	std::ofstream file(dataFileName);
	std::stringstream stringStream;
	std::string str1;
	int i = 0;
	for (;i < 3; i++)
	{
		// 最初だけ見出しを付ける
		if (i <= 0)
		{
			file << "<Data=";
		}
		// strVecに格納された文字列を書き込んでいく
		file << strVec[i];

		// 最後になったら改行
		if (i >= 2)
		{
			file << std::endl;
			file << "/Data>";
		}
		else
		{
			// ｺﾝﾏで区切る
			file << ",";
		}
	}
	for (; i < strVec.size(); i++)
	{
		// 最初だけ見出しを付ける
		if (i <= 3)
		{
			// 改行
			file << std::endl;
			// 見出し
			file << "<Image=";
		}
		// 最後の行じゃなければ
		if (i < strVec.size()-1)
		{
			// 文字列書き込み
			file << strVec[i];
			// ｶﾝﾏ書き込み
			file << ",";
			// 改行
			file << std::endl;
		}
		// 最後の行だとｶﾝﾏは不要なので
		else
		{
			// 文字列書き込み
			file << strVec[i];
			// 改行
			file << std::endl;
		}
	}
	// 最後に付け加える
	file << "/Image>";
	// ﾌｧｲﾙを閉じる
	file.close();
}

void FileManager::WriteRectDataFile(const std::string& fileName, const ColliderBoxManager& boxManager)
{
	std::string dataFileName = "DataFiles/Details/" + fileName + ".dat";
	std::ofstream file(dataFileName);

	// int型からstring型に変換するﾗﾑﾀﾞ
	auto GetStr = [&](const int& val) {
		return std::to_string(val);
	};
	// ColliderBoxEditで格納していった変数を利用して
	// 矩形ﾃﾞｰﾀを取得
	auto colliderBoxes = boxManager.GetColliderBoxList();
	// Manager内に入っている矩形全部で回す
	for (int num = 0;num < colliderBoxes.size();num++)
	{
		// 最初に見出し
		file << "<RectNum=";
		// そのｺﾏに矩形が何個存在するかを書き込む
		file << GetStr(colliderBoxes[num].size());
		// 記号書き込み
		file << ">,";
		// 1ｺﾏの中で存在する矩形の数で回す
		for (int i = 0;i < colliderBoxes[num].size();i++)
		{
			// 1矩形ﾃﾞｰﾀ
			auto box = colliderBoxes[num][i];
			// {〇〇,〇〇}みたいな感じで矩形のﾎﾟｼﾞｼｮﾝを書き込む
			file << "{" + GetStr(box->GetPos().x) + "," + GetStr(box->GetPos().y) + "}";
			// ｺﾝﾏ
			file << ",";
			// {〇〇,〇〇}みたいな感じで矩形のｻｲｽﾞを書き込む
			file << "{" + GetStr(box->GetSize().x) + "," + GetStr(box->GetSize().y) + "}";
			// ｶﾝﾏ
			file << ",";
			// 矩形ﾀｲﾌﾟの書き込み--------------------
			auto boolStr = "";
			if (box->GetTypeFlag())
			{
				boolStr = "true";
			}
			else
			{
				boolStr = "false";
			}
			file << boolStr;
			// --------------------------------------
		}
		// 最後の行で無ければｺﾝﾏを付ける
		if (num < colliderBoxes.size() - 1)
		{
			file << ",";
		}
		// 改行
		file << std::endl;
	}
	// 最後に付け加え
	file << "/RectData>";
	// ﾌｧｲﾙを閉じる
	file.close();
}

void FileManager::ClearFileNameList(void)
{
	// ﾌｧｲﾙ名ﾘｽﾄの中身を空にする
	fileNameList_.clear();
	// ｱﾆﾒｰｼｮﾝ名ﾘｽﾄの中身を空にする
	animationNameList_.clear();
}

FileManager::~FileManager()
{
}
