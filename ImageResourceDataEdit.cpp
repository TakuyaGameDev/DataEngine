#include <DxLib.h>
#include <iostream>
#include <algorithm>

#include "ImageResourceDataEdit.h"
#include "ImageManager.h"
#include "SpriteDataCache.h"
#include "FileManager.h"

#include "Application.h"

#include "Mouse.h"

#include "HitCheck.h"


ImageResourceDataEdit::ImageResourceDataEdit()
{
	exitAppFlag_ = false;
	Initialize();
}

ImageResourceDataEdit::~ImageResourceDataEdit()
{
}

UniqueScene ImageResourceDataEdit::Update(UniqueScene own, const std::shared_ptr<Mouse>& mouse)
{
	// ﾏｳｽのｸﾘｯｸ情報をMouseｸﾗｽから取得
	auto clickData = mouse->GetClick();
	// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
	auto mPoint = mouse->GetPos();

	nowOperation_ = OperationProcess::MAX;

	if (HitCheck_FromCenter()(processPos_[static_cast<int>(OperationProcess::DECISION)], mPoint,
		boxSize_[static_cast<int>(OperationProcess::DECISION)]))
	{
		nowOperation_ = OperationProcess::DECISION;
	}

	if (HitCheck_FromCenter()(processPos_[static_cast<int>(OperationProcess::RETURN)], mPoint,
		boxSize_[static_cast<int>(OperationProcess::RETURN)]))
	{
		nowOperation_ = OperationProcess::RETURN;
	}

	updater_[nowPhase_](mouse);


	ClsDrawScreen();
	Draw();
	ScreenFlip();
	return move(own);
}

void ImageResourceDataEdit::Initialize(void)
{
	// FileManagerの生成(singletonｲﾝｽﾀﾝｽ)
	FileManager::Create();
	// SpriteDataCacheの生成(singletonｲﾝｽﾀﾝｽ)
	SpriteDataCache::Create();
	lpImageManager.Load("Images/button_decide.png");
	lpImageManager.Load("Images/button_return.png");
	lpImageManager.Load("Images/inputButton.png");
	auto port = lpApplication.GetViewport();
	processPos_ = { Vector2I{port.screen.x / 2 + 200,port.screen.y - 200},
		Vector2I{port.screen.x / 2 + 200,port.screen.y - 100} };
	inputBoxPos_ = Vector2I{ port.screen.x / 2 - 150,60 };
	inputBoxSize_ = Vector2I{ 500,25 };
	inputStringSize_ = Vector2I{ 50,25 };
	inputStringPos_ = 
		Vector2I{ inputBoxPos_.x + (inputBoxSize_.x / 2) + (inputStringSize_.x/2),inputBoxPos_.y};

	boxSize_ = { Vector2I{105,50},Vector2I{95,50} };

	nowOperation_ = OperationProcess::MAX;
	nowPhase_ = Phase::FIRST;
	excecuteProcess_ = nowOperation_;

	inputDirectoryFlag_ = false;
	onInputDirectoryButton_ = false;
	saveCmpFlag_ = false;
	selectAnimNo_ = -1;
	animNoOnMouse_ = selectAnimNo_;

	inputFunc_ = &ImageResourceDataEdit::InputDirectoryName;

	InitializeUpdater();
	InitializeDrawExcecuter();

}

void ImageResourceDataEdit::InitializeUpdater(void)
{
	updater_.try_emplace(Phase::FIRST, [&](const std::shared_ptr<Mouse>& mouse) {

		// ﾏｳｽのｸﾘｯｸ情報をMouseｸﾗｽから取得
		auto clickData = mouse->GetClick();
		// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
		auto mPoint = mouse->GetPos();
		if (HitCheck_FromCenter()(inputStringPos_, mPoint, inputBoxSize_))
		{
			onInputDirectoryButton_ = true;
		}
		else
		{
			onInputDirectoryButton_ = false;
		}

		for (auto click : clickData)
		{
			// ｸﾘｯｸﾄﾘｶﾞｰ有り
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (onInputDirectoryButton_)
				{
					inputDirectoryFlag_ = true;
				}
			}
		}
		if (inputDirectoryFlag_)
		{
			(this->*inputFunc_)();
		}
		});

	updater_.try_emplace(Phase::SECOND, [&](const std::shared_ptr<Mouse>& mouse) {
		// ﾏｳｽのｸﾘｯｸ情報をMouseｸﾗｽから取得
		auto clickData = mouse->GetClick();
		// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
		auto mPoint = mouse->GetPos();
		// 常にﾏｳｽﾎﾟｲﾝﾀが乗っているﾌｧｲﾙ名番号は-1にしておく
		// こうすることによってﾏｳｽﾎﾟｲﾝﾀがﾌｧｲﾙ番号上に来た時にこの番号が更新される仕組み
		animNoOnMouse_ = -1;
		// 表示されているﾌｧｲﾙ名全てで回す
		for(int num = 0;num < animNamePos_.size();num++)
		{
			// 表示されているﾌｧｲﾙ名とﾏｳｽﾎﾟｲﾝﾀの当たり判定(左端起点として当たり判定をしている)
			if (HitCheck_FromLeftUp()(animNamePos_[num], mPoint, animNameSize_[num]))
			{
				// ﾏｳｽﾎﾟｲﾝﾀが来た番号で更新
				animNoOnMouse_ = num;
			}
		}
		for (auto click : clickData)
		{
			// ｸﾘｯｸﾄﾘｶﾞｰ有り
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (animNoOnMouse_ != -1)
				{
					// ｸﾘｯｸされていたら番号を更新
					selectAnimNo_ = animNoOnMouse_;
				}
				// 【決定】ﾎﾞﾀﾝ押下時
				if (nowOperation_ == OperationProcess::DECISION)
				{
					nowPhase_ = Phase::THIRD;
				}
				// 【戻る】ﾎﾞﾀﾝ押下時
				if (nowOperation_ == OperationProcess::RETURN)
				{
					selectAnimNo_ = -1;
				}
			}
		}
		});

	updater_.try_emplace(Phase::THIRD, [&](const std::shared_ptr<Mouse>& mouse) {

		// FileManagerｸﾗｽよりDataFileから読み取ったｱﾆﾒｰｼｮﾝ名を取得
		// ﾏｳｽで選んだｱﾆﾒｰｼｮﾝ名
		auto animName = lpFileManager.GetAnimationNameList()[selectAnimNo_];
		// 画像ﾃﾞｨﾚｸﾄﾘ名の作成
		auto imageFileName = "Images/Actor/" + (std::string)inputObjectName_ + "/" + animName;
		// 画像ﾃﾞｨﾚｸﾄﾘ名より画像ﾌｧｲﾙ名の検索
		lpFileManager.SearchFileFromDirectory(imageFileName.c_str());

		});

	updater_.try_emplace(Phase::FOURTH, [&](const std::shared_ptr<Mouse>& mouse) {
		// ﾏｳｽのｸﾘｯｸ情報をMouseｸﾗｽから取得
		auto clickData = mouse->GetClick();
		// ﾏｳｽﾎﾟｲﾝﾀのﾎﾟｼﾞｼｮﾝ
		auto mPoint = mouse->GetPos();
		for (auto click : clickData)
		{
			// ｸﾘｯｸﾄﾘｶﾞｰ有り
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (nowOperation_ == OperationProcess::DECISION)
				{
					// ｾｰﾌﾞする処理
					// FileManagerに渡すﾃﾞｰﾀ文字列
					std::vector<std::string> strVec;
					// 最大ﾌﾚｰﾑ値の格納
					strVec.push_back((std::string)inputMaxFrame_);
					// ﾙｰﾌﾟﾌﾗｸﾞの格納
					strVec.push_back((std::string)inputLoopFlag_);
					// 1ｺﾏにかかる時間の格納
					strVec.push_back((std::string)inputDuration_);

					// 画像ﾊﾟｽ名の格納
					for (auto fileName : lpFileManager.GetFileNameList())
					{
						strVec.push_back(fileName);
					}
					// 編集しているｱﾆﾒｰｼｮﾝ名
					auto animName = lpFileManager.GetAnimationNameList()[selectAnimNo_];
					// 作成ﾃﾞｰﾀをﾃﾞｰﾀﾌｧｲﾙに書き込む
					lpFileManager.WriteImageDataFile((std::string)inputObjectName_ + "/Rect/" + animName, strVec);
					saveCmpFlag_ = true;
				}
				if (nowOperation_ == OperationProcess::RETURN)
				{
					// やり直しをする処理
					nowPhase_ = Phase::FIRST;
					// ﾌｧｲﾙ名群の中を空にする
					lpFileManager.ClearFileNameList();
					// 入力ﾀｰﾝを初期状態に変更
					inputFunc_ = &ImageResourceDataEdit::InputDirectoryName;
					// 各種入力文字列の初期化
					for (int i = 0; i < 20; i++)
					{
						inputDirectoryName_[i] = ' ';
						inputObjectName_[i] = ' ';
						inputMaxFrame_[i] = ' ';
						inputMaxFrame_[i] = ' ';
						inputLoopFlag_[i] = ' ';
						inputDuration_[i] = ' ';
					}
				}
			}
		}
		});
}

void ImageResourceDataEdit::InitializeDrawExcecuter(void)
{
	drawExcecuter_.try_emplace(Phase::FIRST, [&]() {
		DrawFormatString(inputBoxPos_.x - (inputBoxSize_.x / 2), inputBoxPos_.y - (inputBoxSize_.y / 2) - 30,
			0xffffff, "ﾌｧｲﾙﾃﾞｨﾚｸﾄﾘ名を入力して下さい(※ﾌﾙﾊﾟｽ)");

		DrawBox(inputBoxPos_.x - (inputBoxSize_.x / 2), inputBoxPos_.y - (inputBoxSize_.y / 2),
			inputBoxPos_.x + (inputBoxSize_.x / 2), inputBoxPos_.y + (inputBoxSize_.y / 2),
			0xffffff, false);

		DrawRotaGraph(inputStringPos_.x, inputStringPos_.y,1.0f,0.0f,
			lpImageManager.GetHandle("Images/inputButton.png")[0], true);

		// ﾏｳｽﾎﾟｲﾝﾀが所定の場所に来たら透過された箱を描画
		if (onInputDirectoryButton_)
		{
			// 透過させるためにｱﾙﾌｧﾌﾞﾚﾝﾄﾞする(そこそこ薄く箱を描画させたいため150という値)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(inputStringPos_.x - (inputStringSize_.x/2),inputStringPos_.y - (inputStringSize_.y/2),
				inputStringPos_.x + (inputStringSize_.x / 2), inputStringPos_.y + (inputStringSize_.y / 2), 
				0x00ff00, true);
			// ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを箱だけに適応させたいので、箱の描画の最後に0でﾘｾｯﾄをかける
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		});

	drawExcecuter_.try_emplace(Phase::SECOND, [&]() {
		auto animNames = lpFileManager.GetAnimationNameList();
		for (int num = 0; num < animNames.size();num++)
		{
			// 無限にpush_backしないようにｶﾞｰﾄﾞを張る
			if (animNamePos_.size() < animNames.size())
			{
				// ﾌｧｲﾙ名ｻｲｽﾞを詰めていく
				animNameSize_.push_back(Vector2I{ (int)animNames[num].size() * 9,20 });
				// ﾌｧｲﾙ名のﾎﾟｼﾞｼｮﾝを詰めていく
				animNamePos_.push_back(Vector2I{ 50,50 + (num * 25)});
			}
			// ﾌｧｲﾙ名表示
			DrawFormatString(animNamePos_[num].x, animNamePos_[num].y, 
				0xffffff, "%s", animNames[num].c_str());
		}
		if (animNoOnMouse_ != -1)
		{
			// 透過させるためにｱﾙﾌｧﾌﾞﾚﾝﾄﾞする(そこそこ薄く箱を描画させたいため150という値)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(animNamePos_[animNoOnMouse_].x,animNamePos_[animNoOnMouse_].y,
				animNamePos_[animNoOnMouse_].x + animNameSize_[animNoOnMouse_].x,
				animNamePos_[animNoOnMouse_].y + animNameSize_[animNoOnMouse_].y,
				0xf0f0f0, true);
			// ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを箱だけに適応させたいので、箱の描画の最後に0でﾘｾｯﾄをかける
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatString(0, 0, 0xffffff, "%d", selectAnimNo_);
		});

	drawExcecuter_.try_emplace(Phase::THIRD, [&]() {
		DrawFormatString(100, 100, 0xffffff, "ｱﾆﾒｰｼｮﾝ最大ﾌﾚｰﾑ数を入力");
		DrawFormatString(100, 130, 0xffffff, "%s", inputMaxFrame_);

		DrawFormatString(100, 200, 0xffffff, "ﾙｰﾌﾟﾌﾗｸﾞ【true or false】)を入力");
		DrawFormatString(100, 230, 0xffffff, "%s", inputLoopFlag_);

		DrawFormatString(100, 300, 0xffffff, "1ｺﾏにかかる時間(float型)を入力");
		DrawFormatString(100, 330, 0xffffff, "%s", inputDuration_);

		(this->*inputFunc_)();
		});
	drawExcecuter_.try_emplace(Phase::FOURTH, [&]() {
		DrawFormatString(50, 50, 0xffffff, "このﾃﾞｰﾀでよろしいですか？よろしければ【決定】やり直したい場合【戻る】をｸﾘｯｸ");

		DrawFormatString(50, 100, 0xffffff, "最大ﾌﾚｰﾑ数");
		auto maxFrame = lpSpriteDataCache.GetAnimationData().maxFrame;
		DrawFormatString(50, 130, 0x00ff00, "%d", maxFrame);

		DrawFormatString(50, 160, 0xffffff, "loopﾌﾗｸﾞ");
		bool loop = lpSpriteDataCache.GetAnimationData().loop;
		std::string str = "false";
		if (loop)
		{
			str = "true";
		}
		DrawFormatString(50, 190, 0x00ff00, "%s", str.c_str());

		DrawFormatString(50, 220, 0xffffff, "1ｺﾏにかかる時間");
		auto duration = lpSpriteDataCache.GetAnimationData().duration;
		DrawFormatString(50, 250, 0x00ff00, "%lf", duration);

		DrawFormatString(50, 280, 0xffffff, "画像ﾊﾟｽ群");
		// 画像ﾊﾟｽ名群
		auto fileNames = lpFileManager.GetFileNameList();
		for (int num = 0;num < fileNames.size();num++)
		{
			DrawFormatString(50, 300 + (num * 20), 0x00ff00, "%s", fileNames[num].c_str());
		}
		if (saveCmpFlag_)
		{
			DrawFormatString(processPos_[static_cast<int>(OperationProcess::DECISION)].x,
				processPos_[static_cast<int>(OperationProcess::DECISION)].y - 100, 0xff0000, "ｾｰﾌﾞしました！");
		}
		});
}

void ImageResourceDataEdit::Draw(void)
{
	drawExcecuter_[nowPhase_]();
	DrawRotaGraph(processPos_[static_cast<int>(OperationProcess::DECISION)].x,
		processPos_[static_cast<int>(OperationProcess::DECISION)].y, 1.0f, 0.0f,
		lpImageManager.GetHandle("Images/button_decide.png")[0], true);

	DrawRotaGraph(processPos_[static_cast<int>(OperationProcess::RETURN)].x,
		processPos_[static_cast<int>(OperationProcess::RETURN)].y, 1.0f, 0.0f,
		lpImageManager.GetHandle("Images/button_return.png")[0], true);
	if (nowPhase_ == Phase::FIRST && inputDirectoryName_)
	{
		DrawFormatString(inputBoxPos_.x - (inputBoxSize_.x / 2),
			inputBoxPos_.y - (inputBoxSize_.y / 2), 0xffffff, inputDirectoryName_);
	}
	// ﾏｳｽﾎﾟｲﾝﾀが所定の場所に来たら透過された箱を描画
	if (nowOperation_ != OperationProcess::MAX)
	{
		// 透過させるためにｱﾙﾌｧﾌﾞﾚﾝﾄﾞする(そこそこ薄く箱を描画させたいため150という値)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(processPos_[static_cast<int>(nowOperation_)].x - (boxSize_[static_cast<int>(nowOperation_)].x / 2),
			processPos_[static_cast<int>(nowOperation_)].y - (boxSize_[static_cast<int>(nowOperation_)].y / 2),
			processPos_[static_cast<int>(nowOperation_)].x + (boxSize_[static_cast<int>(nowOperation_)].x / 2),
			processPos_[static_cast<int>(nowOperation_)].y + (boxSize_[static_cast<int>(nowOperation_)].y / 2), 0xff0000, true);
		// ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを箱だけに適応させたいので、箱の描画の最後に0でﾘｾｯﾄをかける
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ImageResourceDataEdit::InputDirectoryName(void)
{
	// ﾃﾞｨﾚｸﾄﾘ名の入力
	inputHandle_[0] = KeyInputSingleCharString(inputBoxPos_.x - (inputBoxSize_.x / 2),
		inputBoxPos_.y - (inputBoxSize_.y / 2), 100, inputDirectoryName_, true);
	// ｵﾌﾞｼﾞｪｸﾄ名入力に移行
	inputFunc_ = &ImageResourceDataEdit::InputObjectName;
}

void ImageResourceDataEdit::InputObjectName(void)
{
	// ｵﾌﾞｼﾞｪｸﾄ名の入力
	inputHandle_[1] = KeyInputSingleCharString(inputBoxPos_.x - (inputBoxSize_.x / 2) + 100,
		inputBoxPos_.y - (inputBoxSize_.y / 2), 100, inputObjectName_, true);
	// ﾃﾞｨﾚｸﾄﾘ名をstring型にｷｬｽﾄ
	auto dirName = (std::string)inputDirectoryName_;
	// ﾌｧｲﾙﾊﾟｽをﾃﾞｨﾚｸﾄﾘ名と入力したｵﾌﾞｼﾞｪｸﾄ名で生成
	auto filePath = dirName + (std::string)inputObjectName_;
	// DataFile/よりﾃﾞｰﾀﾌｧｲﾙの読み込み
	lpFileManager.ReadDataFile(filePath);
	// ﾌｧｲﾙ名ｾﾚｸﾄに移行
	nowPhase_ = Phase::SECOND;
	inputFunc_ = &ImageResourceDataEdit::InputMaxFrame;
}

void ImageResourceDataEdit::InputMaxFrame(void)
{
	// 最大ﾌﾚｰﾑ値の入力
	inputHandle_[2] = KeyInputSingleCharString(100, 130, 100, inputMaxFrame_, true);
	// 入力した文字をstring型にｷｬｽﾄ
	auto str = (std::string)inputMaxFrame_;
	// 入力した値のｾｯﾄ(std::atoi->string型をint型に変更)
	lpSpriteDataCache.SetMaxFrame(std::atoi(str.c_str()));
	inputFunc_ = &ImageResourceDataEdit::InputLoopFlag;
}

void ImageResourceDataEdit::InputLoopFlag(void)
{
	// loopFlag(ｱﾆﾒｰｼｮﾝをﾙｰﾌﾟ再生させるか)の入力(true or false)
	bool flg = false;
	inputHandle_[3] = KeyInputSingleCharString(100, 230, 100, inputLoopFlag_, true);
	// 入力した文字をstring型にｷｬｽﾄ
	auto loopStr = (std::string)inputLoopFlag_;
	// 入力した値が"true"だったら
	if (loopStr == "true")
	{
		// ｾｯﾄするﾌﾗｸﾞをtrueに
		flg = true;
	}
	// 入力した値のｾｯﾄ
	lpSpriteDataCache.SetLoopFlag(flg);
	inputFunc_ = &ImageResourceDataEdit::InputDuration;
}

void ImageResourceDataEdit::InputDuration(void)
{
	// duration(1ｺﾏにかかる時間)の入力
	inputHandle_[4] = KeyInputSingleCharString(100, 330, 100, inputDuration_, true);
	std::string durationStr = inputDuration_;
	// 入力した値のｾｯﾄ(std::stof -> string型をfloat型に変更)
	lpSpriteDataCache.SetDuration(std::stof(durationStr));
	// 処理段階の移行
	nowPhase_ = Phase::FOURTH;
}
