#pragma once
#include <memory>
// 各ｼｰﾝの親ｸﾗｽとなる

class BaseScene;
class Mouse;

using UniqueScene = std::unique_ptr<BaseScene>;

enum class Phase
{
	// 1段階目
    FIRST,
	// 2段階目
    SECOND,
	// 3段階目
    THIRD,
	// 4段階目
    FOURTH,
    MAX
};

class BaseScene
{
public:
	BaseScene();
	~BaseScene();
	// 各ｼｰﾝの更新
	virtual UniqueScene Update(UniqueScene own,const std::shared_ptr<Mouse>& mouse) = 0;
	// 各種変数の初期化
	virtual void Initialize(void) = 0;
	// ﾌｪｰｽﾞ毎の処理の初期化
	virtual void InitializeUpdater(void) = 0;
	// ﾌｪｰｽﾞ毎の描画処理の初期化
	virtual void InitializeDrawExcecuter(void) = 0;
	// 各ｼｰﾝの描画
	virtual void Draw(void) = 0;

	// ｱﾌﾟﾘｹｰｼｮﾝ終了を知らせるﾌﾗｸﾞ取得
	const bool& ExitApp(void)const
	{
		return exitAppFlag_;
	}
private:
	
protected:
	// ｱﾌﾟﾘｹｰｼｮﾝ終了ﾌﾗｸﾞ
	bool exitAppFlag_;
};

