#include <DxLib.h>

#include "ColliderBoxEdit.h"
#include "ImageResourceDataEdit.h"
#include "ColliderBoxManager.h"
#include "ColliderBox.h"
#include "FileManager.h"
#include "Mouse.h"

#include "ImageManager.h"
#include "Application.h"
#include "HitCheck.h"

ColliderBoxEdit::ColliderBoxEdit()
{
	// ｱﾌﾟﾘｹｰｼｮﾝ終了ﾌﾗｸﾞの初期化
	exitAppFlag_ = false;
	// 各種変数の初期化
	Initialize();
}

ColliderBoxEdit::~ColliderBoxEdit()
{
	// このｼｰﾝが破棄される際にﾘｾｯﾄさせる
	ResetData();
}

UniqueScene ColliderBoxEdit::Update(UniqueScene own, const std::shared_ptr<Mouse>& mouse)
{
	// ｸﾘｯｸ情報
	auto clickData = mouse->GetClick();
	// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
	mPoint_ = mouse->GetPos();
	// 左右ｸﾘｯｸ変数を常にMAXにしておく
	// しておくことで、常に右ｸﾘｯｸされている状態や左ｸﾘｯｸされている状態を避けるため
	clickDir_ = CLICK::MAX;
	// ｸﾘｯｸ情報で回す
	for (auto click : clickData)
	{
		// ｸﾘｯｸﾄﾘｶﾞｰ有り
		if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
		{
			// 左ｸﾘｯｸ時
			if (click.first == INPUT_ID::START)
			{
				clickDir_ = CLICK::LEFT;
			}
			// 右ｸﾘｯｸ時
			if (click.first == INPUT_ID::RESET)
			{
				clickDir_ = CLICK::RIGHT;
			}
		}
	}
	// 特定のｱﾆﾒｰｼｮﾝ名やｵﾌﾞｼﾞｪｸﾄ名でﾃﾞｰﾀﾌｧｲﾙ検索してﾃﾞｰﾀﾌｧｲﾙが見つからなかったとき
	if (!isFindFile_)
	{
		// ImageData編集ﾎﾞﾀﾝにﾏｳｽｶｰｿﾙが来たら
		if (HitCheck_FromCenter()(goImageDataButtonPos_, mPoint_, goImageDataButtonSize_))
		{
			// ﾏｳｽｶｰｿﾙが上に来たﾌﾗｸﾞをtrueに
			isOnReturn_ = true;
			// 左ｸﾘｯｸ時
			if (clickDir_ == CLICK::LEFT)
			{
				// ImageDataEditに移行
				return std::make_unique<ImageResourceDataEdit>();
			}
		}
		else
		{
			// ﾏｳｽｶｰｿﾙが来てない
			isOnReturn_ = false;
		}
		// ﾏｳｽｶｰｿﾙがｱﾌﾟﾘｹｰｼｮﾝ終了ﾎﾞﾀﾝの上に来た時
		if (HitCheck_FromCenter()(exitAppButtonPos_, mPoint_, exitAppButtonSize_))
		{
			// ﾏｳｽｶｰｿﾙが上に来たﾌﾗｸﾞをtrueに
			isOnExitApp_ = true;
			// 左ｸﾘｯｸ時
			if (clickDir_ == CLICK::LEFT)
			{
				// ｱﾌﾟﾘｹｰｼｮﾝ終了ﾌﾗｸﾞをtrueにしｱﾌﾟﾘｹｰｼｮﾝを終了
				exitAppFlag_ = true;
			}
		}
		else
		{
			// ﾏｳｽｶｰｿﾙが来てない
			isOnExitApp_ = false;
		}
	}
	
	ClsDrawScreen();
	// 描画
	Draw();
	ScreenFlip();
	// 更新
	updater_[nowPhase_](mouse);
	return move(own);
}

void ColliderBoxEdit::Initialize(void)
{
	// FileManagerの生成(Singletonｲﾝｽﾀﾝｽ)
	FileManager::Create();
	// 初期入力ﾀｰﾝをｵﾌﾞｼﾞｪｸﾄ名の入力にする
	inputFunc_ = &ColliderBoxEdit::InputObjectName;
	// 初期編集ﾀｰﾝをｱﾆﾒｰｼｮﾝのｺﾏを選択させるﾀｰﾝにする
	editFunc_ = &ColliderBoxEdit::SelectFrame;
	// 現在の段階をFIRSTにする
	nowPhase_ = Phase::FIRST;
	// 各画面ｻｲｽﾞの取得
	auto vp = lpApplication.GetViewport();

	// 各ﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝやｻｲｽﾞの初期化--------------------------------------
	objectNameBoxPos_ = { vp.screen.x / 2 - 100, vp.screen.y / 2 - 100 };
	objectNameBoxSize_ = { 100,25 };
	animationNameBoxPos_ = { vp.screen.x / 2 - 100, vp.screen.y / 2 + 50 };
	animationNameBoxSize_ = { 100,25 };
	frameNumBoxSize_ = { 35,35 };
	typeChangerPos_ = { Vector2I{ 200,100 },Vector2I{200,130} };
	typeChangerBoxSize_ = { 100,25 };
	saveButtonPos_ = { 100,vp.screen.y - 100 };
	saveButtonSize_ = { 65,25 };
	goImageDataButtonPos_ = { vp.screen.x / 2,vp.screen.y / 2 };
	goImageDataButtonSize_ = { 335,30 };
	exitAppButtonPos_ = { vp.screen.x / 2,vp.screen.y / 2 + 100 };
	exitAppButtonSize_ = { 175,26 };
	objImagePos_ = { vp.editScreen.x - 100 + ((vp.screen.x-(vp.editScreen.x - 100))/2),
		vp.editScreen.y - 100 + ((vp.screen.y - (vp.editScreen.y - 100)) / 2) };
	// --------------------------------------------------------------------------
	// ｱﾆﾒｰｼｮﾝｺﾏを表示している所にﾏｳｽﾎﾟｲﾝﾀが来た時に更新される数字
	// そのｺﾏ数と同じ数字になる
	onPointFrame_ = -1;
	// 最終的に決定したｱﾆﾒｰｼｮﾝｺﾏの初期化
	selectedFrame_ = onPointFrame_;
	// ﾃﾞｰﾀﾌｧｲﾙ検索ﾌﾗｸﾞの初期化
	isFindFile_ = false;
	// 描く矩形のﾀｲﾌﾟ(Attack(true) or Damage(false))
	typeFlag_ = false;
	// ImageDataEditに戻るﾎﾞﾀﾝの上に
	// ﾏｳｽﾎﾟｲﾝﾀが来たﾌﾗｸﾞの初期化
	isOnReturn_ = false;
	// ｱﾌﾟﾘｹｰｼｮﾝ終了ﾎﾞﾀﾝの上にﾏｳｽﾎﾟｲﾝﾀが来たﾌﾗｸﾞの初期化
	isOnExitApp_ = false;
	// 更新関数達の格納
	InitializeUpdater();
	// 描画関数達の格納
	InitializeDrawExcecuter();

	// 各画像のﾛｰﾄﾞ ----------------------------------------------
	lpImageManager.Load("Images/saveButton.png");
	lpImageManager.Load("Images/goImageDataEditButton.png");
	lpImageManager.Load("Images/exit_application.png");
	// -----------------------------------------------------------
}

void ColliderBoxEdit::InitializeUpdater(void)
{
	// このｼｰﾝの1番目に行われる更新処理
	updater_.try_emplace(Phase::FIRST, [&](const std::shared_ptr<Mouse>& mouse) {
		// 各種入力処理
		(this->*inputFunc_)();
		});
	// このｼｰﾝの2番目に行われる更新処理
	updater_.try_emplace(Phase::SECOND, [&](const std::shared_ptr<Mouse>& mouse) {
		// ﾃﾞｰﾀﾌｧｲﾙの検索(入力ｵﾌﾞｼﾞｪｸﾄ名とｱﾆﾒｰｼｮﾝ名で)
		isFindFile_ = lpFileManager.IsExistFile("Sprite",inputObjectName_, inputAnimationName_);
		// ﾃﾞｰﾀﾌｧｲﾙが存在する
		if (isFindFile_)
		{
			// 入力下ｱﾆﾒｰｼｮﾝ名をstringにｷｬｽﾄ
			auto animName = (std::string)inputAnimationName_;
			// ﾌﾙﾊﾟｽに生成
			auto fullPath = "Images/Actor/" + (std::string)inputObjectName_ + "/" + animName;
			// 入力したｵﾌﾞｼﾞｪｸﾄ名とｱﾆﾒｰｼｮﾝ名からﾌｫﾙﾀﾞ内に存在するﾌｧｲﾙを検索
			lpFileManager.SearchFileFromDirectory(fullPath);
			// ﾃﾞｰﾀﾌｧｲﾙから読み取った画像ﾊﾟｽで画像のﾛｰﾄﾞ
			for (auto& fileName : lpFileManager.GetFileNameList())
			{
				lpImageManager.Load("Images/Actor/" + (std::string)inputObjectName_ + 
					"/" + animName + "/" + fileName);
			}
			// ﾌﾚｰﾑ数を表示する箱のresize
			frameNumBox_.resize(lpFileManager.GetFileNameList().size());
			// 箱のﾎﾟｼﾞｼｮﾝとそこに書いている数字の設定
			for (int i = 0; i < frameNumBox_.size(); i++)
			{
				frameNumBox_[i].first = { 50 + (i * 50),30 };
				frameNumBox_[i].second = i;
			}
			// 次の処理ﾀｰﾝに移行
			nowPhase_ = Phase::THIRD;
		}
		});
	// このｼｰﾝの3番目に行われる更新処理
	updater_.try_emplace(Phase::THIRD, [&](const std::shared_ptr<Mouse>& mouse) {
		// 編集の際に行われる更新処理(引数:左右ｸﾘｯｸ,ﾏｳｽﾎﾟｲﾝﾀ)
		(this->*editFunc_)(clickDir_,mPoint_);
		});
}

void ColliderBoxEdit::InitializeDrawExcecuter(void)
{
	// このｼｰﾝの1番目に行われる更新処理
	drawExcecuter_.try_emplace(Phase::FIRST, [&]() {
		// ｵﾌﾞｼﾞｪｸﾄ名の入力ﾌｫｰﾑ
		DrawFormatString(objectNameBoxPos_.x, objectNameBoxPos_.y - 25, 0xffffff, "ｵﾌﾞｼﾞｪｸﾄ名を入力");
		DrawBox(objectNameBoxPos_.x, objectNameBoxPos_.y,
			objectNameBoxPos_.x + objectNameBoxSize_.x, objectNameBoxPos_.y + objectNameBoxSize_.y,
			0xffffff, false);
		// ｱﾆﾒｰｼｮﾝ名の入力ﾌｫｰﾑ
		DrawFormatString(animationNameBoxPos_.x, animationNameBoxPos_.y - 25, 0xffffff, "ｱﾆﾒｰｼｮﾝ名を入力");
		DrawBox(animationNameBoxPos_.x, animationNameBoxPos_.y,
			animationNameBoxPos_.x + animationNameBoxSize_.x, animationNameBoxPos_.y + animationNameBoxSize_.y,
			0xffffff, false);
		// 入力したｵﾌﾞｼﾞｪｸﾄ名の表示
		DrawFormatString(objectNameBoxPos_.x, objectNameBoxPos_.y, 0xffffff, "%s", inputObjectName_);
		// 入力したｱﾆﾒｰｼｮﾝ名の表示
		DrawFormatString(animationNameBoxPos_.x, animationNameBoxPos_.y, 0xffffff,"%s",inputAnimationName_);

		});
	// このｼｰﾝの2番目に行われる更新処理
	drawExcecuter_.try_emplace(Phase::SECOND, [&]() {
		if (!isFindFile_)
		{
			// ﾃﾞｰﾀﾌｧｲﾙ検索する際の文字列
			auto str = (std::string)inputObjectName_ + "_" + (std::string)inputAnimationName_;
			// ﾃﾞｰﾀがない事を知らせる
			DrawFormatString(0, 0, 0xffffff,
				"ﾃﾞｰﾀが有りません！%s%のImageData編集を先に終了させて下さい。",
				str.c_str());
			// ImageDataEditに戻るためのﾎﾞﾀﾝの描画
			DrawRotaGraph(goImageDataButtonPos_.x, goImageDataButtonPos_.y,1.0f,0.0f,
				lpImageManager.GetHandle("Images/goImageDataEditButton.png")[0],
				true);
			// ｱﾌﾟﾘｹｰｼｮﾝ終了するﾎﾞﾀﾝの描画
			DrawRotaGraph(exitAppButtonPos_.x, exitAppButtonPos_.y, 1.0f, 0.0f,
				lpImageManager.GetHandle("Images/exit_application.png")[0],
				true);

			// 特定の場所にﾏｳｽﾎﾟｲﾝﾀが来た時にそこに半透明のﾎﾞｯｸｽを描画------------
			// ImageDataEditに戻るﾎﾞﾀﾝ上
			if (isOnReturn_)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				DrawBox(goImageDataButtonPos_.x - (goImageDataButtonSize_.x / 2),
					goImageDataButtonPos_.y - (goImageDataButtonSize_.y / 2),
					goImageDataButtonPos_.x + (goImageDataButtonSize_.x / 2),
					goImageDataButtonPos_.y + (goImageDataButtonSize_.y / 2), 0xff0000,true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			// ｱﾌﾟﾘｹｰｼｮﾝ終了のﾎﾞﾀﾝ上
			if (isOnExitApp_)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				DrawBox(exitAppButtonPos_.x - (exitAppButtonSize_.x / 2),
					exitAppButtonPos_.y - (exitAppButtonSize_.y / 2),
					exitAppButtonPos_.x + (exitAppButtonSize_.x / 2),
					exitAppButtonPos_.y + (exitAppButtonSize_.y / 2), 0x0000ff, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			// ---------------------------------------------------------------------
		}
		});
	// このｼｰﾝの3番目に行われる更新処理
	drawExcecuter_.try_emplace(Phase::THIRD, [&]() {
		// Applicationｸﾗｽより各画面ｻｲｽﾞの取得
		auto vp = lpApplication.GetViewport();
		// 背景
		DrawBox(0, 0, vp.screen.x, vp.screen.y, 0x000fff, true);
		// ｷｬﾗ画像のｽｸﾘｰﾝ背景
		DrawBox(vp.editScreen.x - 100, vp.editScreen.y - 100,
			vp.screen.x, vp.screen.y, 0x000000, true);
		// ﾌﾚｰﾑを表示させている上に描画
		DrawFormatString(50,0, 0xffffff, "各ﾌﾚｰﾑ↓");
		// 各ﾌﾚｰﾑﾎﾞﾀﾝを1,2,3,4,5,6,7......と描画していく
		for (int i = 0;i < frameNumBox_.size();i++)
		{
			DrawBox(frameNumBox_[i].first.x, frameNumBox_[i].first.y,
				frameNumBox_[i].first.x + frameNumBoxSize_.x,
				frameNumBox_[i].first.y + frameNumBoxSize_.y,
				0xffffff, false);
			DrawFormatString(frameNumBox_[i].first.x + 10,
				frameNumBox_[i].first.y + 10, 0xffffff, "%d", frameNumBox_[i].second);
			// saveFlag_がﾘｻｲｽﾞされてから描画開始
			if (saveFlag_.size() > 0)
			{
				// そのｺﾏがｾｰﾌﾞ完了していたら上に赤色で「済」という文字を表示
				if (saveFlag_[i])
				{
					DrawFormatString(frameNumBox_[i].first.x + 10, frameNumBox_[i].first.y - 15,
						0xff0000, "済");
				}
			}
		}
		// ﾏｳｽﾎﾟｲﾝﾀがそのﾌﾚｰﾑのﾎﾞﾀﾝ上に来たら
		// ﾏｳｽﾎﾟｲﾝﾀが来たﾌﾚｰﾑﾎﾞﾀﾝを薄い緑で塗りつぶす
		if (onPointFrame_ > -1)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawBox(frameNumBox_[onPointFrame_].first.x, frameNumBox_[onPointFrame_].first.y,
				frameNumBox_[onPointFrame_].first.x + frameNumBoxSize_.x,
				frameNumBox_[onPointFrame_].first.y + frameNumBoxSize_.y, 0x00ff00, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		// 編集対象ｷｬﾗ画像の描画と編集対象画像のﾌｧｲﾙ名の描画
		if (selectedFrame_ != -1)
		{
			auto fileName = "Images/Actor/" + (std::string)inputObjectName_ +
				"/" + (std::string)inputAnimationName_ + "/" + lpFileManager.GetFileNameList()[selectedFrame_];
			DrawRotaGraph(objImagePos_.x, objImagePos_.y, 1.0f, 0.0f, lpImageManager.GetHandle(fileName)[0], true);
			DrawFormatString(vp.editScreen.x - 100, vp.editScreen.y - 125, 0xffffff, "%s",fileName.c_str());
		}
		// 矩形編集のﾀｰﾝだと
		if (editFunc_ == &ColliderBoxEdit::BoxEdit)
		{
			// ﾏｳｽﾎﾟｲﾝﾀの先に矩形ｻｲｽﾞ毎の色で小さな丸を塗りつぶしで描画
			DrawCircle(mPoint_.x, mPoint_.y, 5, typeColor_, true);
		}
		// 矩形ﾀｲﾌﾟの変更ﾎﾞﾀﾝの描画-------------------------------------------------------------
		DrawFormatString(typeChangerPos_[0].x - 80, typeChangerPos_[0].y, 0xffffff, "矩形ﾀｲﾌﾟ:");
		for (auto typePos : typeChangerPos_)
		{
			DrawBox(typePos.x, typePos.y,
				typePos.x + typeChangerBoxSize_.x, typePos.y + typeChangerBoxSize_.y, 0xffffff, false);
		}
		DrawFormatString(typeChangerPos_[0].x + 5, typeChangerPos_[0].y + 5, 0xff0000, "攻撃矩形");
		DrawFormatString(typeChangerPos_[1].x + 5, typeChangerPos_[1].y + 5, 0x00ff00, "ﾀﾞﾒｰｼﾞ矩形");
		// -------------------------------------------------------------------------------------
		// 矩形ﾀｲﾌﾟ番号
		auto typeNo = -1;
		// 攻撃矩形だと
		if (typeFlag_)
		{
			// 0に
			typeNo = 0;
		}
		// ﾀﾞﾒｰｼﾞ矩形だと
		else
		{
			// 1に
			typeNo = 1;
		}
		// 攻撃矩形かﾀﾞﾒｰｼﾞ矩形ならば
		if (typeNo != -1)
		{
			// 現在選択ﾀｲﾌﾟの横に←描画
			// 攻撃矩形　←　や　ﾀﾞﾒｰｼﾞ矩形　← みたいに描画させる
			DrawFormatString(typeChangerPos_[typeNo].x + 120, typeChangerPos_[typeNo].y + 5,
				0xffffff, "←", true,10);
		}
		// boxManager_が生成されていて、ﾌﾚｰﾑ数が設定されていたら
		if (boxManager_ && selectedFrame_ != -1)
		{
			// 矩形の始点
			auto begPos = boxManager_->GetBegPos();
			// 矩形の始点がｴﾗｰﾎﾟｼﾞｼｮﾝ上にない場合
			if (begPos.x > -1 && begPos.y > -1)
			{
				// 画像編集画面(黒色の画面)にﾏｳｽが有れば
				if (HitCheck_FromLeftUp()
					(Vector2I(vp.editScreen.x - 100, vp.editScreen.y - 100), mPoint_,
					 Vector2I((vp.screen.x - (vp.editScreen.x - 100)),
							 ((vp.screen.y - (vp.editScreen.y - 100))))))
				{
					// 現在編集している矩形の描画
					DrawBox(begPos.x, begPos.y, mPoint_.x, mPoint_.y, typeColor_, false);
				}
			}
			// boxManager_内に追加されたcolliderBox達の描画(指定したﾌﾚｰﾑの矩形で)
			boxManager_->Draw(selectedFrame_);
		}
		// ｾｰﾌﾞﾎﾞﾀﾝの描画
		DrawGraph(saveButtonPos_.x, saveButtonPos_.y,
			lpImageManager.GetHandle("Images/saveButton.png")[0], true);
		// ｾｰﾌﾞﾎﾞﾀﾝの周りに箱を描画
		DrawBox(saveButtonPos_.x, saveButtonPos_.y,
			saveButtonPos_.x + saveButtonSize_.x, saveButtonPos_.y + saveButtonSize_.y,
			0xffffff, false);
		});

}

void ColliderBoxEdit::Draw(void)
{
	// 各段階毎の描画
	drawExcecuter_[nowPhase_]();
}

void ColliderBoxEdit::InputObjectName(void)
{
	// ｵﾌﾞｼﾞｪｸﾄ名の入力
	if (KeyInputSingleCharString(objectNameBoxPos_.x, objectNameBoxPos_.y, 20, inputObjectName_, true) == 1)
	{
		// 終了時にｱﾆﾒｰｼｮﾝ名入力に移行
		inputFunc_ = &ColliderBoxEdit::InputAnimationName;
	}
}

void ColliderBoxEdit::InputAnimationName(void)
{
	// ｱﾆﾒｰｼｮﾝ名の入力
	if (KeyInputSingleCharString(animationNameBoxPos_.x, animationNameBoxPos_.y, 20, inputAnimationName_, true) == 1)
	{
		// 次の処理ﾌﾞﾛｯｸに移行
		nowPhase_ = Phase::SECOND;
	}
}

void ColliderBoxEdit::SelectFrame(const CLICK& click, const Vector2I& mPoint)
{
	// ﾏｳｽﾎﾟｲﾝﾀがどのﾌﾚｰﾑ数のﾎﾞﾀﾝに来ているかの変数に
	// ｴﾗｰ値を格納させる(常に)
	onPointFrame_ = -1;
	// ﾌﾚｰﾑの数だけで回す
	for (auto frameBox : frameNumBox_)
	{
		// ﾏｳｽﾎﾟｲﾝﾀが特定のﾌﾚｰﾑ番号の上に来た時
		if (HitCheck_FromLeftUp()(frameBox.first, mPoint, frameNumBoxSize_))
		{
			// ﾏｳｽﾎﾟｲﾝﾀが上に来たﾌﾚｰﾑを格納
			onPointFrame_ = frameBox.second;
			// 左ｸﾘｯｸ時
			if (clickDir_ == CLICK::LEFT)
			{
				// 選択ﾌﾚｰﾑをﾏｳｽﾎﾟｲﾝﾀが来ているﾌﾚｰﾑにする
				selectedFrame_ = onPointFrame_;
				if (!boxManager_)
				{
					// ﾎﾞｯｸｽﾏﾈｰｼﾞｬｰの生成
					boxManager_ = new ColliderBoxManager(frameNumBox_.size());
				}

				saveFlag_.resize(frameNumBox_.size());
				// 箱の編集段階に移行
				editFunc_ = &ColliderBoxEdit::BoxEdit;
			}
		}
	}
}

void ColliderBoxEdit::BoxEdit(const CLICK& click, const Vector2I& mPoint)
{
	// 矩形ﾀｲﾌﾟの変更------------------------------------------------------------
	// 0:攻撃矩形ﾀｲﾌﾟ　1:ﾀﾞﾒｰｼﾞ矩形ﾀｲﾌﾟ
	if (HitCheck_FromLeftUp()(typeChangerPos_[0], mPoint, typeChangerBoxSize_))
	{
		if (click == CLICK::LEFT)
		{
			typeFlag_ = true;
		}
	}
	if (HitCheck_FromLeftUp()(typeChangerPos_[1], mPoint, typeChangerBoxSize_))
	{
		if (click == CLICK::LEFT)
		{
			typeFlag_ = false;
		}
	}
	// ---------------------------------------------------------------------------
	// 攻撃矩形編集だと
	if (typeFlag_)
	{
		// 赤に
		typeColor_ = 0xff0000;
	}
	// ﾀﾞﾒｰｼﾞ矩形編集だと
	else
	{
		// 青に
		typeColor_ = 0x00ff00;
	}
	// 各画面ｻｲｽﾞの取得
	auto vp = lpApplication.GetViewport();
	// 編集画面上にﾏｳｽﾎﾟｲﾝﾀが存在すれば
	if (HitCheck_FromLeftUp()
		(Vector2I(vp.editScreen.x - 100, vp.editScreen.y - 100), mPoint_,
			Vector2I((vp.screen.x - (vp.editScreen.x - 100)),
				((vp.screen.y - (vp.editScreen.y - 100))))))
	{
		// 左ｸﾘｯｸ時
		if (click == CLICK::LEFT)
		{
			// 矩形の始点の設定
			boxManager_->SetBegPos(mPoint);
		}
		// 右ｸﾘｯｸ時
		if (click == CLICK::RIGHT)
		{
			// 終点が決まった事を意味するので
			// この時点でcolliderBoxのAdd
			boxManager_->AddColliderBox(selectedFrame_, mPoint, typeFlag_);
		}
	}
	// ｾｰﾌﾞﾎﾞﾀﾝ上にﾏｳｽﾎﾟｲﾝﾀが存在すれば
	if (HitCheck_FromLeftUp()(saveButtonPos_, mPoint_, saveButtonSize_))
	{
		// 左ｸﾘｯｸ時
		if (click == CLICK::LEFT)
		{
			// 現在の矩形ﾃﾞｰﾀをｾｰﾌﾞしたﾌﾗｸﾞをtrueに
			saveFlag_[selectedFrame_] = true;
			// ｱﾆﾒｰｼｮﾝｺﾏ数選択に変更
			editFunc_ = &ColliderBoxEdit::SelectFrame;
			// ﾏｳｽﾎﾟｲﾝﾀが存在するﾌﾚｰﾑ数の初期化
			onPointFrame_ = -1;
			// 決定時のﾌﾚｰﾑ数の初期化
			selectedFrame_ = onPointFrame_;
			// 全てのﾌﾚｰﾑ画像の編集の終了ﾌﾗｸﾞ全てで回す
			auto flg = true;
			for (auto frameFlg : saveFlag_)
			{
				// 1つでもfalseだとfalseになるように論理積を取る
				flg &= frameFlg;
			}
			// 全てのﾌﾚｰﾑ画像が編集終了していたら
			if (flg)
			{
				// ｾｰﾌﾞするﾃﾞｰﾀﾌｧｲﾙ名
				std::string fileName = (std::string)inputObjectName_ + "/Rect/" + (std::string)inputAnimationName_;
				// ﾃﾞｰﾀﾌｧｲﾙを作成し、その中にﾃﾞｰﾀを書き込んでいく
				lpFileManager.WriteRectDataFile(fileName, *boxManager_);
				// 各変数等のﾘｾｯﾄ
				ResetData();
			}
		}
	}
}

void ColliderBoxEdit::ResetData(void)
{
	// 初期入力ﾀｰﾝをｵﾌﾞｼﾞｪｸﾄ名の入力にする
	inputFunc_ = &ColliderBoxEdit::InputObjectName;
	// 初期編集ﾀｰﾝをﾌﾚｰﾑ選択のﾀｰﾝにする
	editFunc_ = &ColliderBoxEdit::SelectFrame;
	// 各入力名の初期化
	for (int i = 0; i < 20; i++)
	{
		inputObjectName_[i] = ' ';
		inputAnimationName_[i] = ' ';
	}
	// boxManagerのdelete(開放)
	delete boxManager_;
	// boxManager_を空にしておく
	boxManager_ = nullptr;
	onPointFrame_ = -1;
	selectedFrame_ = onPointFrame_;
	// ﾌﾚｰﾑﾎﾞﾀﾝのﾎﾟｼﾞｼｮﾝｺﾝﾃﾅを空にする
	frameNumBox_.clear();
	// ｾｰﾌﾞ完了ﾌﾗｸﾞの中身を空にする
	saveFlag_.clear();
	// ﾃﾞｰﾀﾌｧｲﾙ検索ﾌﾗｸﾞの初期化
	isFindFile_ = false;
	// 矩形ﾀｲﾌﾟ変数の初期化
	typeFlag_ = false;
	// 処理段階を初期に戻す
	nowPhase_ = Phase::FIRST;
}
