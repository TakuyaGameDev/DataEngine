#pragma once
#include <map>
#include <string>
#include <vector>

#include "Geometory.h"

using VecInt = std::vector<int>;

#define lpImageManager ImageManager::getInstance()

// 画像のﾛｰﾄﾞや画像ﾊﾝﾄﾞﾙの取得等を目的としたｸﾗｽ
// ｼﾝｸﾞﾙﾄﾝｲﾝｽﾀﾝｽ
class ImageManager
{
public:
	static ImageManager& getInstance(void)
	{
		static ImageManager sInstance_;
		return sInstance_;
	}
	// 画像単発ﾛｰﾄﾞ
	void Load(std::string name);
	// 画像分割ﾛｰﾄﾞ
	void Load(std::string name, Vector2I divcnt, Vector2I divsize);

	// 画像ﾊﾝﾄﾞﾙ取得
	// param@ name: 画像ﾌｧｲﾙ名
	const VecInt& GetHandle(std::string name)
	{
		return handles_[name];
	}

private:
	ImageManager() = default;
	// ｺﾋﾟｰ禁止
	void operator=(const ImageManager&) = delete;
	// ﾛｰﾄﾞ時に画像ﾊﾝﾄﾞﾙを格納する変数
	// 画像ﾌｧｲﾙ名で取得しやすいようにstd::map<std::string>にしている
	std::map<std::string, VecInt> handles_;
};

