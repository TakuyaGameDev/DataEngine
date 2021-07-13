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
	// Ï³½‚Ì¸Ø¯¸î•ñ‚ğMouse¸×½‚©‚çæ“¾
	auto clickData = mouse->GetClick();
	// Ï³½Îß²İÀ‚ÌÎß¼Ş¼®İ
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
	// FileManager‚Ì¶¬(singleton²İ½Àİ½)
	FileManager::Create();
	// SpriteDataCache‚Ì¶¬(singleton²İ½Àİ½)
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

		// Ï³½‚Ì¸Ø¯¸î•ñ‚ğMouse¸×½‚©‚çæ“¾
		auto clickData = mouse->GetClick();
		// Ï³½Îß²İÀ‚ÌÎß¼Ş¼®İ
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
			// ¸Ø¯¸ÄØ¶Ş°—L‚è
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
		// Ï³½‚Ì¸Ø¯¸î•ñ‚ğMouse¸×½‚©‚çæ“¾
		auto clickData = mouse->GetClick();
		// Ï³½Îß²İÀ‚ÌÎß¼Ş¼®İ
		auto mPoint = mouse->GetPos();
		// í‚ÉÏ³½Îß²İÀ‚ªæ‚Á‚Ä‚¢‚éÌ§²Ù–¼”Ô†‚Í-1‚É‚µ‚Ä‚¨‚­
		// ‚±‚¤‚·‚é‚±‚Æ‚É‚æ‚Á‚ÄÏ³½Îß²İÀ‚ªÌ§²Ù”Ô†ã‚É—ˆ‚½‚É‚±‚Ì”Ô†‚ªXV‚³‚ê‚éd‘g‚İ
		animNoOnMouse_ = -1;
		// •\¦‚³‚ê‚Ä‚¢‚éÌ§²Ù–¼‘S‚Ä‚Å‰ñ‚·
		for(int num = 0;num < animNamePos_.size();num++)
		{
			// •\¦‚³‚ê‚Ä‚¢‚éÌ§²Ù–¼‚ÆÏ³½Îß²İÀ‚Ì“–‚½‚è”»’è(¶’[‹N“_‚Æ‚µ‚Ä“–‚½‚è”»’è‚ğ‚µ‚Ä‚¢‚é)
			if (HitCheck_FromLeftUp()(animNamePos_[num], mPoint, animNameSize_[num]))
			{
				// Ï³½Îß²İÀ‚ª—ˆ‚½”Ô†‚ÅXV
				animNoOnMouse_ = num;
			}
		}
		for (auto click : clickData)
		{
			// ¸Ø¯¸ÄØ¶Ş°—L‚è
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (animNoOnMouse_ != -1)
				{
					// ¸Ø¯¸‚³‚ê‚Ä‚¢‚½‚ç”Ô†‚ğXV
					selectAnimNo_ = animNoOnMouse_;
				}
				// yŒˆ’èzÎŞÀİ‰Ÿ‰º
				if (nowOperation_ == OperationProcess::DECISION)
				{
					nowPhase_ = Phase::THIRD;
				}
				// y–ß‚ézÎŞÀİ‰Ÿ‰º
				if (nowOperation_ == OperationProcess::RETURN)
				{
					selectAnimNo_ = -1;
				}
			}
		}
		});

	updater_.try_emplace(Phase::THIRD, [&](const std::shared_ptr<Mouse>& mouse) {

		// FileManager¸×½‚æ‚èDataFile‚©‚ç“Ç‚İæ‚Á‚½±ÆÒ°¼®İ–¼‚ğæ“¾
		// Ï³½‚Å‘I‚ñ‚¾±ÆÒ°¼®İ–¼
		auto animName = lpFileManager.GetAnimationNameList()[selectAnimNo_];
		// ‰æ‘œÃŞ¨Ú¸ÄØ–¼‚Ìì¬
		auto imageFileName = "Images/Actor/" + (std::string)inputObjectName_ + "/" + animName;
		// ‰æ‘œÃŞ¨Ú¸ÄØ–¼‚æ‚è‰æ‘œÌ§²Ù–¼‚ÌŒŸõ
		lpFileManager.SearchFileFromDirectory(imageFileName.c_str());

		});

	updater_.try_emplace(Phase::FOURTH, [&](const std::shared_ptr<Mouse>& mouse) {
		// Ï³½‚Ì¸Ø¯¸î•ñ‚ğMouse¸×½‚©‚çæ“¾
		auto clickData = mouse->GetClick();
		// Ï³½Îß²İÀ‚ÌÎß¼Ş¼®İ
		auto mPoint = mouse->GetPos();
		for (auto click : clickData)
		{
			// ¸Ø¯¸ÄØ¶Ş°—L‚è
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (nowOperation_ == OperationProcess::DECISION)
				{
					// ¾°ÌŞ‚·‚éˆ—
					// FileManager‚É“n‚·ÃŞ°À•¶š—ñ
					std::vector<std::string> strVec;
					// Å‘åÌÚ°Ñ’l‚ÌŠi”[
					strVec.push_back((std::string)inputMaxFrame_);
					// Ù°ÌßÌ×¸Ş‚ÌŠi”[
					strVec.push_back((std::string)inputLoopFlag_);
					// 1ºÏ‚É‚©‚©‚éŠÔ‚ÌŠi”[
					strVec.push_back((std::string)inputDuration_);

					// ‰æ‘œÊß½–¼‚ÌŠi”[
					for (auto fileName : lpFileManager.GetFileNameList())
					{
						strVec.push_back(fileName);
					}
					// •ÒW‚µ‚Ä‚¢‚é±ÆÒ°¼®İ–¼
					auto animName = lpFileManager.GetAnimationNameList()[selectAnimNo_];
					// ì¬ÃŞ°À‚ğÃŞ°ÀÌ§²Ù‚É‘‚«‚Ş
					lpFileManager.WriteImageDataFile((std::string)inputObjectName_ + "/Rect/" + animName, strVec);
					saveCmpFlag_ = true;
				}
				if (nowOperation_ == OperationProcess::RETURN)
				{
					// ‚â‚è’¼‚µ‚ğ‚·‚éˆ—
					nowPhase_ = Phase::FIRST;
					// Ì§²Ù–¼ŒQ‚Ì’†‚ğ‹ó‚É‚·‚é
					lpFileManager.ClearFileNameList();
					// “ü—ÍÀ°İ‚ğ‰Šúó‘Ô‚É•ÏX
					inputFunc_ = &ImageResourceDataEdit::InputDirectoryName;
					// Šeí“ü—Í•¶š—ñ‚Ì‰Šú‰»
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
			0xffffff, "Ì§²ÙÃŞ¨Ú¸ÄØ–¼‚ğ“ü—Í‚µ‚Ä‰º‚³‚¢(¦ÌÙÊß½)");

		DrawBox(inputBoxPos_.x - (inputBoxSize_.x / 2), inputBoxPos_.y - (inputBoxSize_.y / 2),
			inputBoxPos_.x + (inputBoxSize_.x / 2), inputBoxPos_.y + (inputBoxSize_.y / 2),
			0xffffff, false);

		DrawRotaGraph(inputStringPos_.x, inputStringPos_.y,1.0f,0.0f,
			lpImageManager.GetHandle("Images/inputButton.png")[0], true);

		// Ï³½Îß²İÀ‚ªŠ’è‚ÌêŠ‚É—ˆ‚½‚ç“§‰ß‚³‚ê‚½” ‚ğ•`‰æ
		if (onInputDirectoryButton_)
		{
			// “§‰ß‚³‚¹‚é‚½‚ß‚É±ÙÌ§ÌŞÚİÄŞ‚·‚é(‚»‚±‚»‚±”–‚­” ‚ğ•`‰æ‚³‚¹‚½‚¢‚½‚ß150‚Æ‚¢‚¤’l)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(inputStringPos_.x - (inputStringSize_.x/2),inputStringPos_.y - (inputStringSize_.y/2),
				inputStringPos_.x + (inputStringSize_.x / 2), inputStringPos_.y + (inputStringSize_.y / 2), 
				0x00ff00, true);
			// ±ÙÌ§ÌŞÚİÄŞ‚ğ” ‚¾‚¯‚É“K‰‚³‚¹‚½‚¢‚Ì‚ÅA” ‚Ì•`‰æ‚ÌÅŒã‚É0‚ÅØ¾¯Ä‚ğ‚©‚¯‚é
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		});

	drawExcecuter_.try_emplace(Phase::SECOND, [&]() {
		auto animNames = lpFileManager.GetAnimationNameList();
		for (int num = 0; num < animNames.size();num++)
		{
			// –³ŒÀ‚Épush_back‚µ‚È‚¢‚æ‚¤‚É¶Ş°ÄŞ‚ğ’£‚é
			if (animNamePos_.size() < animNames.size())
			{
				// Ì§²Ù–¼»²½Ş‚ğ‹l‚ß‚Ä‚¢‚­
				animNameSize_.push_back(Vector2I{ (int)animNames[num].size() * 9,20 });
				// Ì§²Ù–¼‚ÌÎß¼Ş¼®İ‚ğ‹l‚ß‚Ä‚¢‚­
				animNamePos_.push_back(Vector2I{ 50,50 + (num * 25)});
			}
			// Ì§²Ù–¼•\¦
			DrawFormatString(animNamePos_[num].x, animNamePos_[num].y, 
				0xffffff, "%s", animNames[num].c_str());
		}
		if (animNoOnMouse_ != -1)
		{
			// “§‰ß‚³‚¹‚é‚½‚ß‚É±ÙÌ§ÌŞÚİÄŞ‚·‚é(‚»‚±‚»‚±”–‚­” ‚ğ•`‰æ‚³‚¹‚½‚¢‚½‚ß150‚Æ‚¢‚¤’l)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(animNamePos_[animNoOnMouse_].x,animNamePos_[animNoOnMouse_].y,
				animNamePos_[animNoOnMouse_].x + animNameSize_[animNoOnMouse_].x,
				animNamePos_[animNoOnMouse_].y + animNameSize_[animNoOnMouse_].y,
				0xf0f0f0, true);
			// ±ÙÌ§ÌŞÚİÄŞ‚ğ” ‚¾‚¯‚É“K‰‚³‚¹‚½‚¢‚Ì‚ÅA” ‚Ì•`‰æ‚ÌÅŒã‚É0‚ÅØ¾¯Ä‚ğ‚©‚¯‚é
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatString(0, 0, 0xffffff, "%d", selectAnimNo_);
		});

	drawExcecuter_.try_emplace(Phase::THIRD, [&]() {
		DrawFormatString(100, 100, 0xffffff, "±ÆÒ°¼®İÅ‘åÌÚ°Ñ”‚ğ“ü—Í");
		DrawFormatString(100, 130, 0xffffff, "%s", inputMaxFrame_);

		DrawFormatString(100, 200, 0xffffff, "Ù°ÌßÌ×¸Şytrue or falsez)‚ğ“ü—Í");
		DrawFormatString(100, 230, 0xffffff, "%s", inputLoopFlag_);

		DrawFormatString(100, 300, 0xffffff, "1ºÏ‚É‚©‚©‚éŠÔ(floatŒ^)‚ğ“ü—Í");
		DrawFormatString(100, 330, 0xffffff, "%s", inputDuration_);

		(this->*inputFunc_)();
		});
	drawExcecuter_.try_emplace(Phase::FOURTH, [&]() {
		DrawFormatString(50, 50, 0xffffff, "‚±‚ÌÃŞ°À‚Å‚æ‚ë‚µ‚¢‚Å‚·‚©H‚æ‚ë‚µ‚¯‚ê‚ÎyŒˆ’èz‚â‚è’¼‚µ‚½‚¢ê‡y–ß‚éz‚ğ¸Ø¯¸");

		DrawFormatString(50, 100, 0xffffff, "Å‘åÌÚ°Ñ”");
		auto maxFrame = lpSpriteDataCache.GetAnimationData().maxFrame;
		DrawFormatString(50, 130, 0x00ff00, "%d", maxFrame);

		DrawFormatString(50, 160, 0xffffff, "loopÌ×¸Ş");
		bool loop = lpSpriteDataCache.GetAnimationData().loop;
		std::string str = "false";
		if (loop)
		{
			str = "true";
		}
		DrawFormatString(50, 190, 0x00ff00, "%s", str.c_str());

		DrawFormatString(50, 220, 0xffffff, "1ºÏ‚É‚©‚©‚éŠÔ");
		auto duration = lpSpriteDataCache.GetAnimationData().duration;
		DrawFormatString(50, 250, 0x00ff00, "%lf", duration);

		DrawFormatString(50, 280, 0xffffff, "‰æ‘œÊß½ŒQ");
		// ‰æ‘œÊß½–¼ŒQ
		auto fileNames = lpFileManager.GetFileNameList();
		for (int num = 0;num < fileNames.size();num++)
		{
			DrawFormatString(50, 300 + (num * 20), 0x00ff00, "%s", fileNames[num].c_str());
		}
		if (saveCmpFlag_)
		{
			DrawFormatString(processPos_[static_cast<int>(OperationProcess::DECISION)].x,
				processPos_[static_cast<int>(OperationProcess::DECISION)].y - 100, 0xff0000, "¾°ÌŞ‚µ‚Ü‚µ‚½I");
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
	// Ï³½Îß²İÀ‚ªŠ’è‚ÌêŠ‚É—ˆ‚½‚ç“§‰ß‚³‚ê‚½” ‚ğ•`‰æ
	if (nowOperation_ != OperationProcess::MAX)
	{
		// “§‰ß‚³‚¹‚é‚½‚ß‚É±ÙÌ§ÌŞÚİÄŞ‚·‚é(‚»‚±‚»‚±”–‚­” ‚ğ•`‰æ‚³‚¹‚½‚¢‚½‚ß150‚Æ‚¢‚¤’l)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(processPos_[static_cast<int>(nowOperation_)].x - (boxSize_[static_cast<int>(nowOperation_)].x / 2),
			processPos_[static_cast<int>(nowOperation_)].y - (boxSize_[static_cast<int>(nowOperation_)].y / 2),
			processPos_[static_cast<int>(nowOperation_)].x + (boxSize_[static_cast<int>(nowOperation_)].x / 2),
			processPos_[static_cast<int>(nowOperation_)].y + (boxSize_[static_cast<int>(nowOperation_)].y / 2), 0xff0000, true);
		// ±ÙÌ§ÌŞÚİÄŞ‚ğ” ‚¾‚¯‚É“K‰‚³‚¹‚½‚¢‚Ì‚ÅA” ‚Ì•`‰æ‚ÌÅŒã‚É0‚ÅØ¾¯Ä‚ğ‚©‚¯‚é
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ImageResourceDataEdit::InputDirectoryName(void)
{
	// ÃŞ¨Ú¸ÄØ–¼‚Ì“ü—Í
	inputHandle_[0] = KeyInputSingleCharString(inputBoxPos_.x - (inputBoxSize_.x / 2),
		inputBoxPos_.y - (inputBoxSize_.y / 2), 100, inputDirectoryName_, true);
	// µÌŞ¼Şª¸Ä–¼“ü—Í‚ÉˆÚs
	inputFunc_ = &ImageResourceDataEdit::InputObjectName;
}

void ImageResourceDataEdit::InputObjectName(void)
{
	// µÌŞ¼Şª¸Ä–¼‚Ì“ü—Í
	inputHandle_[1] = KeyInputSingleCharString(inputBoxPos_.x - (inputBoxSize_.x / 2) + 100,
		inputBoxPos_.y - (inputBoxSize_.y / 2), 100, inputObjectName_, true);
	// ÃŞ¨Ú¸ÄØ–¼‚ğstringŒ^‚É·¬½Ä
	auto dirName = (std::string)inputDirectoryName_;
	// Ì§²ÙÊß½‚ğÃŞ¨Ú¸ÄØ–¼‚Æ“ü—Í‚µ‚½µÌŞ¼Şª¸Ä–¼‚Å¶¬
	auto filePath = dirName + (std::string)inputObjectName_;
	// DataFile/‚æ‚èÃŞ°ÀÌ§²Ù‚Ì“Ç‚İ‚İ
	lpFileManager.ReadDataFile(filePath);
	// Ì§²Ù–¼¾Ú¸Ä‚ÉˆÚs
	nowPhase_ = Phase::SECOND;
	inputFunc_ = &ImageResourceDataEdit::InputMaxFrame;
}

void ImageResourceDataEdit::InputMaxFrame(void)
{
	// Å‘åÌÚ°Ñ’l‚Ì“ü—Í
	inputHandle_[2] = KeyInputSingleCharString(100, 130, 100, inputMaxFrame_, true);
	// “ü—Í‚µ‚½•¶š‚ğstringŒ^‚É·¬½Ä
	auto str = (std::string)inputMaxFrame_;
	// “ü—Í‚µ‚½’l‚Ì¾¯Ä(std::atoi->stringŒ^‚ğintŒ^‚É•ÏX)
	lpSpriteDataCache.SetMaxFrame(std::atoi(str.c_str()));
	inputFunc_ = &ImageResourceDataEdit::InputLoopFlag;
}

void ImageResourceDataEdit::InputLoopFlag(void)
{
	// loopFlag(±ÆÒ°¼®İ‚ğÙ°ÌßÄ¶‚³‚¹‚é‚©)‚Ì“ü—Í(true or false)
	bool flg = false;
	inputHandle_[3] = KeyInputSingleCharString(100, 230, 100, inputLoopFlag_, true);
	// “ü—Í‚µ‚½•¶š‚ğstringŒ^‚É·¬½Ä
	auto loopStr = (std::string)inputLoopFlag_;
	// “ü—Í‚µ‚½’l‚ª"true"‚¾‚Á‚½‚ç
	if (loopStr == "true")
	{
		// ¾¯Ä‚·‚éÌ×¸Ş‚ğtrue‚É
		flg = true;
	}
	// “ü—Í‚µ‚½’l‚Ì¾¯Ä
	lpSpriteDataCache.SetLoopFlag(flg);
	inputFunc_ = &ImageResourceDataEdit::InputDuration;
}

void ImageResourceDataEdit::InputDuration(void)
{
	// duration(1ºÏ‚É‚©‚©‚éŠÔ)‚Ì“ü—Í
	inputHandle_[4] = KeyInputSingleCharString(100, 330, 100, inputDuration_, true);
	std::string durationStr = inputDuration_;
	// “ü—Í‚µ‚½’l‚Ì¾¯Ä(std::stof -> stringŒ^‚ğfloatŒ^‚É•ÏX)
	lpSpriteDataCache.SetDuration(std::stof(durationStr));
	// ˆ—’iŠK‚ÌˆÚs
	nowPhase_ = Phase::FOURTH;
}
