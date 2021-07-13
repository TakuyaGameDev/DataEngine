#pragma once
#include <map>
#include <array>

#include "Geometory.h"

// 入力ID
enum class INPUT_ID
{
	// ｽﾀｰﾄ
	START,
	// ﾘｾｯﾄ
	RESET,
	MAX
};

// INPUT_IDをfor文で回すための準備
// INPUT_IDのbegin
static INPUT_ID begin(INPUT_ID)
{
	return INPUT_ID::START;
}
// INPUT_IDのend
static INPUT_ID end(INPUT_ID)
{
	return INPUT_ID::MAX;
}
// INPUT_IDのｲﾝｸﾘﾒﾝﾄ
static INPUT_ID operator++(INPUT_ID& id)
{
	return (id =
		INPUT_ID(std::underlying_type<INPUT_ID>::type(id) + 1));
}
// INPUT_IDのﾎﾟｲﾝﾀ返し
static INPUT_ID& operator*(INPUT_ID& id)
{
	return id;
}

// ｸﾘｯｸ時のﾄﾘｶﾞｰにて使用
// 今ｸﾘｯｸしたのか1ﾌﾚｰﾑ前にｸﾘｯｸしたのか
enum class TRG
{
	// 【今】を表す
	NOW,
	// 【1ﾌﾚｰﾑ前】を表す
	OLD,
	MAX
};
// 【今】と【1ﾌﾚｰﾑ前】で配列確保し、
// 【今】ｸﾘｯｸしているﾌﾗｸﾞ、【1ﾌﾚｰﾑ前】にｸﾘｯｸしたﾌﾗｸﾞを保持
using TrgBool = std::array<bool, static_cast<int>(TRG::MAX)>;
// 【今】ｸﾘｯｸしているものが右ｸﾘｯｸなのか左ｸﾘｯｸなのか
// 判別させるためにstd::mapで連想配列にしている
using TrgMap = std::map<INPUT_ID, TrgBool>;

// 入力ﾃﾞﾊﾞｲｽ(ﾏｳｽ)ｸﾗｽ
class Mouse
{
public:
	Mouse();
	~Mouse();
	// 更新
	void Update(void);
	// ﾏｳｽｺﾏﾝﾄﾞのｾｯﾄｱｯﾌﾟ
	void SetUp(void);
	// 入力ﾃﾞｰﾀの取得
	const TrgMap& GetClick(void)
	{
		return inputData_;
	}
	// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ取得
	const Vector2I& GetPos(void)
	{
		return pos_;
	}

private:
	// 入力ﾃﾞｰﾀ
	TrgMap inputData_;
	// ｺﾏﾝﾄﾞﾃｰﾌﾞﾙ
	std::array<int, static_cast<int>(INPUT_ID::MAX)> commandTable_;
	// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
	Vector2I pos_;
};

