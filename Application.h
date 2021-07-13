#pragma once
#include <memory>
#include "Geometory.h"
#include "BaseScene.h"

#define lpApplication Application::getInstance()

class Mouse;

// 画面ｻｲｽﾞ
struct Viewport
{
	// 全画面
	Vector2I screen;
	// 編集画面
	Vector2I editScreen;
};


class Application
{
public:
	// ｼﾝｸﾞﾙﾄﾝｲﾝｽﾀﾝｽ
	static Application& getInstance(void)
	{
		static Application instance_;
		return instance_;
	}

	// ｹﾞｰﾑﾙｰﾌﾟ
	void Run(void);

	// DxLibの終了
	void Exit_DxLib(void);
	// 各画面ｻｲｽﾞ取得
	const Viewport& GetViewport(void)
	{
		return viewport_;
	}

private:
	Application() = default;
	~Application();
	// ｺﾋﾟｰ禁止
	void operator=(const Application&) = delete;
	// DxLibの初期化関係
	bool SysInit(void);
	// 各画面ｻｲｽﾞ設定
	void SetViewport(const Viewport& viewport);
	// 画面ｻｲｽﾞを格納する変数
	Viewport viewport_;
	// ﾏｳｽﾃﾞﾊﾞｲｽｲﾝｽﾀﾝｽ
	std::shared_ptr<Mouse> mouse_;
	// ｼｰﾝｲﾝｽﾀﾝｽ
	UniqueScene scene_;
};

