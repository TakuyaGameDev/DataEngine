#include <DxLib.h>
#include "ImageManager.h"

void ImageManager::Load(std::string name)
{
	// handlesにnameで指定された文字列の画像ﾊﾝﾄﾞﾙが存在しない場合
	// 重複してﾛｰﾄﾞしないようにｶﾞｰﾄﾞをかける
	if (handles_.count(name) <= 0)
	{
		// resizeする
		handles_[name].resize(1);
		// 画像のﾛｰﾄﾞ
		handles_[name][0] = LoadGraph(name.c_str());
	}
}

void ImageManager::Load(std::string name, Vector2I divcnt, Vector2I divsize)
{
	// handlesにnameで指定された文字列の画像ﾊﾝﾄﾞﾙが存在しない場合
	// 重複してﾛｰﾄﾞしないようにｶﾞｰﾄﾞをかける
	if (handles_.count(name) <= 0)
	{
		// 画像ﾊﾝﾄﾞﾙを画像数でresizeする
		handles_[name].resize(divcnt.x * divcnt.y);
		// 画像の分割ﾛｰﾄﾞ
		LoadDivGraph(name.c_str(), divcnt.x * divcnt.y,
			divcnt.x, divcnt.y, divsize.x, divsize.y,
			&handles_[name][0]);
	}
}
