#pragma once
#include <memory>
#include <vector>
#include "Geometory.h"

class ColliderBox;

// shared_ptrで生ﾎﾟｲﾝﾀを防ぐ
using SharedCollider = std::shared_ptr<ColliderBox>;
// ﾌﾚｰﾑ数だけ可変で存在するのでresize可能なようにvector長にしておく
using ColliderVec = std::vector<SharedCollider>;

// 当たり判定矩形をまとめて管理するｸﾗｽ
// こちらの方がまとめて処理しやすい
class ColliderBoxManager
{
public:
	ColliderBoxManager(int maxFrame);
	~ColliderBoxManager();

	// 始点座標のｾｯﾄ
	// ｸﾘｯｸされたらｾｯﾄする
	void SetBegPos(const Vector2I& beg);

	// 矩形の始点の取得
	const Vector2I& GetBegPos(void)const
	{
		return begPos_;
	}

	// colliderBoxList_に追加
	// param@ frame : 現在のﾌﾚｰﾑ(ｺﾏ) boxSize : 箱のｻｲｽﾞ
	void AddColliderBox(const int& frame, const Vector2I& endPos,const bool& typeFlag);

	void Draw(const int& frame);
	// 矩形ﾘｽﾄの取得
	const std::vector<ColliderVec>& GetColliderBoxList(void)const
	{
		return colliderBoxList_;
	}

private:
	// 箱を描画し始める始点座標
	Vector2I begPos_;
	// 1ﾌﾚｰﾑにも複数箱が存在するので可変にしておく
	std::vector<ColliderVec> colliderBoxList_;
};

