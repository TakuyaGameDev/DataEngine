#include <DxLib.h>

#include "Select.h"
#include "Mouse.h"
#include "Application.h"

#include "ImageManager.h"
#include "ImageResourceDataEdit.h"
#include "ColliderBoxEdit.h"

#include "HitCheck.h"

Select::Select()
{
	// ｱﾌﾟﾘｹｰｼｮﾝ終了ﾌﾗｸﾞの初期化
	exitAppFlag_ = false;
	// 各種変数の初期化
	Initialize();
}

Select::~Select()
{
}

UniqueScene Select::Update(UniqueScene own,const std::shared_ptr<Mouse>& mouse)
{
	// 入力ﾃﾞｰﾀをMouseｸﾗｽより取得
	auto clickData = mouse->GetClick();
	// 現在選択中のﾓｰﾄﾞをMAXにする
	// ﾏｳｽﾎﾟｲﾝﾀが所定の場所に行くとこの変数が変更される
	nowMode_ = MODE::MAX;
	// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
	auto mPoint = mouse->GetPos();

	// 当たり判定-------------------------------------------------------------------
	// 画像の中心からの計算
	if (HitCheck_FromCenter()(modePos_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)],
		mPoint, boxSize_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)]))
	{
		// 現在のﾓｰﾄﾞを画像ﾃﾞｰﾀ編集ﾓｰﾄﾞに変更
		nowMode_ = MODE::IMAGERESOURCE_EDITOR;
	}

	if (HitCheck_FromCenter()(modePos_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)],
		mPoint, boxSize_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)]))
	{
		// 現在のﾓｰﾄﾞを当たり判定ﾎﾞｯｸｽ編集ﾓｰﾄﾞに変更
		nowMode_ = MODE::COLLIDERBOX_EDITOR;
	}
	// -----------------------------------------------------------------------------

	for (auto click : clickData)
	{
		// ｸﾘｯｸﾄﾘｶﾞｰ有り
		if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
		{
			// 遷移する先のﾓｰﾄﾞを決定する
			decideMode_ = nowMode_;
		}
		// ﾄﾘｶﾞｰ無し
		if (click.second[static_cast<int>(TRG::NOW)])
		{
			// 遷移する先のﾓｰﾄﾞを決定する
			decideMode_ = nowMode_;
		}
	}
	// 画像ﾃﾞｰﾀ編集に移行
	if (decideMode_ == MODE::IMAGERESOURCE_EDITOR)
	{
		nowMode_ = MODE::MAX;
		decideMode_ = nowMode_;
		return std::make_unique<ImageResourceDataEdit>();
	}
	// 当たり判定ﾎﾞｯｸｽの編集に移行
	if (decideMode_ == MODE::COLLIDERBOX_EDITOR)
	{
		nowMode_ = MODE::MAX;
		decideMode_ = nowMode_;
		return std::make_unique<ColliderBoxEdit>();
	}

	ClsDrawScreen();
	// 描画
	Draw();
	ScreenFlip();
	return move(own);
}

void Select::Initialize(void)
{
	// 各画面ｻｲｽﾞの取得
	auto tmpPort = lpApplication.GetViewport();
	// 移行したいﾓｰﾄﾞﾎﾞﾀﾝの文字列
	modePos_ = { Vector2I{tmpPort.screen.x / 2,tmpPort.screen.y / 2},
				 Vector2I{tmpPort.screen.x / 2,tmpPort.screen.y / 2 + 80} };
	// ﾎﾞﾀﾝのｻｲｽﾞ
	boxSize_ = { Vector2I{430,60},Vector2I{520,60} };
	// 現在のﾓｰﾄﾞ(ﾏｳｽﾎﾟｲﾝﾀが来ているﾓｰﾄﾞ)
	nowMode_ = MODE::MAX;
	// 決定時(左ｸﾘｯｸ時)のﾓｰﾄﾞ
	decideMode_ = nowMode_;
	lpImageManager.Load("Images/modeTitle_ImageData.png");
	lpImageManager.Load("Images/modeTitle_Collider.png");
}

void Select::InitializeUpdater(void)
{
	// 特に処理すること無し
}

void Select::InitializeDrawExcecuter(void)
{
	// 特に処理することなし
}

void Select::Draw(void)
{
	// 移行したい各ﾓｰﾄﾞのﾎﾞﾀﾝの描画--------------------------------------------
	DrawRotaGraph(modePos_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)].x,
		modePos_[static_cast<int>(MODE::IMAGERESOURCE_EDITOR)].y, 1.0f, 0.0f,
		lpImageManager.GetHandle("Images/modeTitle_ImageData.png")[0],true);
	DrawRotaGraph(modePos_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)].x,
		modePos_[static_cast<int>(MODE::COLLIDERBOX_EDITOR)].y, 1.0f, 0.0f,
		lpImageManager.GetHandle("Images/modeTitle_Collider.png")[0],true);
	// ------------------------------------------------------------------------

	// ﾏｳｽﾎﾟｲﾝﾀが所定の場所に来たら透過された箱を描画
	if (nowMode_ != MODE::MAX)
	{
		// 透過させるためにｱﾙﾌｧﾌﾞﾚﾝﾄﾞする(そこそこ薄く箱を描画させたいため150という値)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(modePos_[static_cast<int>(nowMode_)].x - (boxSize_[static_cast<int>(nowMode_)].x / 2),
			modePos_[static_cast<int>(nowMode_)].y - (boxSize_[static_cast<int>(nowMode_)].y / 2),
			modePos_[static_cast<int>(nowMode_)].x + (boxSize_[static_cast<int>(nowMode_)].x / 2),
			modePos_[static_cast<int>(nowMode_)].y + (boxSize_[static_cast<int>(nowMode_)].y / 2), 0xff0000, true);
		// ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを箱だけに適応させたいので、箱の描画の最後に0でﾘｾｯﾄをかける
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
