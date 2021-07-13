#include "ColliderBoxManager.h"
#include "ColliderBox.h"

ColliderBoxManager::ColliderBoxManager(int maxFrame)
{
	// 生成時にﾌﾚｰﾑ数だけresizeしておく
	colliderBoxList_.resize(maxFrame);
	// 始点を0以外であればなんでもいいので
	// とりあえず-1だったら引っかかることもないので
	// この数字で初期化
	begPos_ = { -1,-1 };
}

ColliderBoxManager::~ColliderBoxManager()
{
}

void ColliderBoxManager::SetBegPos(const Vector2I& beg)
{
	// 始点の設定
	begPos_ = beg;
}

void ColliderBoxManager::AddColliderBox(const int& frame, const Vector2I& endPos,const bool& typeFlag)
{
	// ｻｲｽﾞの設定
	auto size = Vector2I(endPos.x - begPos_.x, endPos.y - begPos_.y);
	// 中心座標の設定
	auto center = Vector2I(begPos_.x + (size.x / 2), begPos_.y + (size.y / 2));
	// 矩形の生成
	auto tmpCol = std::make_shared<ColliderBox>(center, size,typeFlag);
	// ﾘｽﾄに追加
	colliderBoxList_[frame].push_back(tmpCol);
	// 始点のﾘｾｯﾄ
	begPos_ = { -1,-1 };
}

void ColliderBoxManager::Draw(const int& frame)
{
	// Addされた矩形の一括描画
	for (auto& box : colliderBoxList_[frame])
	{
		box->Draw();
	}
}
