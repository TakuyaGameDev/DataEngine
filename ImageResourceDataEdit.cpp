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
	// ϳ��̸د�����Mouse�׽����擾
	auto clickData = mouse->GetClick();
	// ϳ��߲�����߼޼��
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
	// FileManager�̐���(singleton�ݽ�ݽ)
	FileManager::Create();
	// SpriteDataCache�̐���(singleton�ݽ�ݽ)
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

		// ϳ��̸د�����Mouse�׽����擾
		auto clickData = mouse->GetClick();
		// ϳ��߲�����߼޼��
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
			// �د��ضް�L��
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
		// ϳ��̸د�����Mouse�׽����擾
		auto clickData = mouse->GetClick();
		// ϳ��߲�����߼޼��
		auto mPoint = mouse->GetPos();
		// ���ϳ��߲��������Ă���̧�ٖ��ԍ���-1�ɂ��Ă���
		// �������邱�Ƃɂ����ϳ��߲����̧�ٔԍ���ɗ������ɂ��̔ԍ����X�V�����d�g��
		animNoOnMouse_ = -1;
		// �\������Ă���̧�ٖ��S�Ăŉ�
		for(int num = 0;num < animNamePos_.size();num++)
		{
			// �\������Ă���̧�ٖ���ϳ��߲���̓����蔻��(���[�N�_�Ƃ��ē����蔻������Ă���)
			if (HitCheck_FromLeftUp()(animNamePos_[num], mPoint, animNameSize_[num]))
			{
				// ϳ��߲���������ԍ��ōX�V
				animNoOnMouse_ = num;
			}
		}
		for (auto click : clickData)
		{
			// �د��ضް�L��
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (animNoOnMouse_ != -1)
				{
					// �د�����Ă�����ԍ����X�V
					selectAnimNo_ = animNoOnMouse_;
				}
				// �y����z���݉�����
				if (nowOperation_ == OperationProcess::DECISION)
				{
					nowPhase_ = Phase::THIRD;
				}
				// �y�߂�z���݉�����
				if (nowOperation_ == OperationProcess::RETURN)
				{
					selectAnimNo_ = -1;
				}
			}
		}
		});

	updater_.try_emplace(Phase::THIRD, [&](const std::shared_ptr<Mouse>& mouse) {

		// FileManager�׽���DataFile����ǂݎ������Ұ��ݖ����擾
		// ϳ��őI�񂾱�Ұ��ݖ�
		auto animName = lpFileManager.GetAnimationNameList()[selectAnimNo_];
		// �摜�ިڸ�ؖ��̍쐬
		auto imageFileName = "Images/Actor/" + (std::string)inputObjectName_ + "/" + animName;
		// �摜�ިڸ�ؖ����摜̧�ٖ��̌���
		lpFileManager.SearchFileFromDirectory(imageFileName.c_str());

		});

	updater_.try_emplace(Phase::FOURTH, [&](const std::shared_ptr<Mouse>& mouse) {
		// ϳ��̸د�����Mouse�׽����擾
		auto clickData = mouse->GetClick();
		// ϳ��߲�����߼޼��
		auto mPoint = mouse->GetPos();
		for (auto click : clickData)
		{
			// �د��ضް�L��
			if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
			{
				if (nowOperation_ == OperationProcess::DECISION)
				{
					// ���ނ��鏈��
					// FileManager�ɓn���ް�������
					std::vector<std::string> strVec;
					// �ő��ڰђl�̊i�[
					strVec.push_back((std::string)inputMaxFrame_);
					// ٰ���׸ނ̊i�[
					strVec.push_back((std::string)inputLoopFlag_);
					// 1�ςɂ����鎞�Ԃ̊i�[
					strVec.push_back((std::string)inputDuration_);

					// �摜�߽���̊i�[
					for (auto fileName : lpFileManager.GetFileNameList())
					{
						strVec.push_back(fileName);
					}
					// �ҏW���Ă����Ұ��ݖ�
					auto animName = lpFileManager.GetAnimationNameList()[selectAnimNo_];
					// �쐬�ް����ް�̧�قɏ�������
					lpFileManager.WriteImageDataFile((std::string)inputObjectName_ + "/Rect/" + animName, strVec);
					saveCmpFlag_ = true;
				}
				if (nowOperation_ == OperationProcess::RETURN)
				{
					// ��蒼�������鏈��
					nowPhase_ = Phase::FIRST;
					// ̧�ٖ��Q�̒�����ɂ���
					lpFileManager.ClearFileNameList();
					// ������݂�������ԂɕύX
					inputFunc_ = &ImageResourceDataEdit::InputDirectoryName;
					// �e����͕�����̏�����
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
			0xffffff, "̧���ިڸ�ؖ�����͂��ĉ�����(�����߽)");

		DrawBox(inputBoxPos_.x - (inputBoxSize_.x / 2), inputBoxPos_.y - (inputBoxSize_.y / 2),
			inputBoxPos_.x + (inputBoxSize_.x / 2), inputBoxPos_.y + (inputBoxSize_.y / 2),
			0xffffff, false);

		DrawRotaGraph(inputStringPos_.x, inputStringPos_.y,1.0f,0.0f,
			lpImageManager.GetHandle("Images/inputButton.png")[0], true);

		// ϳ��߲��������̏ꏊ�ɗ����瓧�߂��ꂽ����`��
		if (onInputDirectoryButton_)
		{
			// ���߂����邽�߂ɱ�̧�����ނ���(����������������`�悳����������150�Ƃ����l)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(inputStringPos_.x - (inputStringSize_.x/2),inputStringPos_.y - (inputStringSize_.y/2),
				inputStringPos_.x + (inputStringSize_.x / 2), inputStringPos_.y + (inputStringSize_.y / 2), 
				0x00ff00, true);
			// ��̧�����ނ𔠂����ɓK�����������̂ŁA���̕`��̍Ō��0��ؾ�Ă�������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		});

	drawExcecuter_.try_emplace(Phase::SECOND, [&]() {
		auto animNames = lpFileManager.GetAnimationNameList();
		for (int num = 0; num < animNames.size();num++)
		{
			// ������push_back���Ȃ��悤�ɶް�ނ𒣂�
			if (animNamePos_.size() < animNames.size())
			{
				// ̧�ٖ����ނ��l�߂Ă���
				animNameSize_.push_back(Vector2I{ (int)animNames[num].size() * 9,20 });
				// ̧�ٖ����߼޼�݂��l�߂Ă���
				animNamePos_.push_back(Vector2I{ 50,50 + (num * 25)});
			}
			// ̧�ٖ��\��
			DrawFormatString(animNamePos_[num].x, animNamePos_[num].y, 
				0xffffff, "%s", animNames[num].c_str());
		}
		if (animNoOnMouse_ != -1)
		{
			// ���߂����邽�߂ɱ�̧�����ނ���(����������������`�悳����������150�Ƃ����l)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(animNamePos_[animNoOnMouse_].x,animNamePos_[animNoOnMouse_].y,
				animNamePos_[animNoOnMouse_].x + animNameSize_[animNoOnMouse_].x,
				animNamePos_[animNoOnMouse_].y + animNameSize_[animNoOnMouse_].y,
				0xf0f0f0, true);
			// ��̧�����ނ𔠂����ɓK�����������̂ŁA���̕`��̍Ō��0��ؾ�Ă�������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatString(0, 0, 0xffffff, "%d", selectAnimNo_);
		});

	drawExcecuter_.try_emplace(Phase::THIRD, [&]() {
		DrawFormatString(100, 100, 0xffffff, "��Ұ��ݍő��ڰѐ������");
		DrawFormatString(100, 130, 0xffffff, "%s", inputMaxFrame_);

		DrawFormatString(100, 200, 0xffffff, "ٰ���׸ށytrue or false�z)�����");
		DrawFormatString(100, 230, 0xffffff, "%s", inputLoopFlag_);

		DrawFormatString(100, 300, 0xffffff, "1�ςɂ����鎞��(float�^)�����");
		DrawFormatString(100, 330, 0xffffff, "%s", inputDuration_);

		(this->*inputFunc_)();
		});
	drawExcecuter_.try_emplace(Phase::FOURTH, [&]() {
		DrawFormatString(50, 50, 0xffffff, "�����ް��ł�낵���ł����H��낵����΁y����z��蒼�������ꍇ�y�߂�z��د�");

		DrawFormatString(50, 100, 0xffffff, "�ő��ڰѐ�");
		auto maxFrame = lpSpriteDataCache.GetAnimationData().maxFrame;
		DrawFormatString(50, 130, 0x00ff00, "%d", maxFrame);

		DrawFormatString(50, 160, 0xffffff, "loop�׸�");
		bool loop = lpSpriteDataCache.GetAnimationData().loop;
		std::string str = "false";
		if (loop)
		{
			str = "true";
		}
		DrawFormatString(50, 190, 0x00ff00, "%s", str.c_str());

		DrawFormatString(50, 220, 0xffffff, "1�ςɂ����鎞��");
		auto duration = lpSpriteDataCache.GetAnimationData().duration;
		DrawFormatString(50, 250, 0x00ff00, "%lf", duration);

		DrawFormatString(50, 280, 0xffffff, "�摜�߽�Q");
		// �摜�߽���Q
		auto fileNames = lpFileManager.GetFileNameList();
		for (int num = 0;num < fileNames.size();num++)
		{
			DrawFormatString(50, 300 + (num * 20), 0x00ff00, "%s", fileNames[num].c_str());
		}
		if (saveCmpFlag_)
		{
			DrawFormatString(processPos_[static_cast<int>(OperationProcess::DECISION)].x,
				processPos_[static_cast<int>(OperationProcess::DECISION)].y - 100, 0xff0000, "���ނ��܂����I");
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
	// ϳ��߲��������̏ꏊ�ɗ����瓧�߂��ꂽ����`��
	if (nowOperation_ != OperationProcess::MAX)
	{
		// ���߂����邽�߂ɱ�̧�����ނ���(����������������`�悳����������150�Ƃ����l)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(processPos_[static_cast<int>(nowOperation_)].x - (boxSize_[static_cast<int>(nowOperation_)].x / 2),
			processPos_[static_cast<int>(nowOperation_)].y - (boxSize_[static_cast<int>(nowOperation_)].y / 2),
			processPos_[static_cast<int>(nowOperation_)].x + (boxSize_[static_cast<int>(nowOperation_)].x / 2),
			processPos_[static_cast<int>(nowOperation_)].y + (boxSize_[static_cast<int>(nowOperation_)].y / 2), 0xff0000, true);
		// ��̧�����ނ𔠂����ɓK�����������̂ŁA���̕`��̍Ō��0��ؾ�Ă�������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ImageResourceDataEdit::InputDirectoryName(void)
{
	// �ިڸ�ؖ��̓���
	inputHandle_[0] = KeyInputSingleCharString(inputBoxPos_.x - (inputBoxSize_.x / 2),
		inputBoxPos_.y - (inputBoxSize_.y / 2), 100, inputDirectoryName_, true);
	// ��޼ު�Ė����͂Ɉڍs
	inputFunc_ = &ImageResourceDataEdit::InputObjectName;
}

void ImageResourceDataEdit::InputObjectName(void)
{
	// ��޼ު�Ė��̓���
	inputHandle_[1] = KeyInputSingleCharString(inputBoxPos_.x - (inputBoxSize_.x / 2) + 100,
		inputBoxPos_.y - (inputBoxSize_.y / 2), 100, inputObjectName_, true);
	// �ިڸ�ؖ���string�^�ɷ���
	auto dirName = (std::string)inputDirectoryName_;
	// ̧���߽���ިڸ�ؖ��Ɠ��͂�����޼ު�Ė��Ő���
	auto filePath = dirName + (std::string)inputObjectName_;
	// DataFile/����ް�̧�ق̓ǂݍ���
	lpFileManager.ReadDataFile(filePath);
	// ̧�ٖ��ڸĂɈڍs
	nowPhase_ = Phase::SECOND;
	inputFunc_ = &ImageResourceDataEdit::InputMaxFrame;
}

void ImageResourceDataEdit::InputMaxFrame(void)
{
	// �ő��ڰђl�̓���
	inputHandle_[2] = KeyInputSingleCharString(100, 130, 100, inputMaxFrame_, true);
	// ���͂���������string�^�ɷ���
	auto str = (std::string)inputMaxFrame_;
	// ���͂����l�̾��(std::atoi->string�^��int�^�ɕύX)
	lpSpriteDataCache.SetMaxFrame(std::atoi(str.c_str()));
	inputFunc_ = &ImageResourceDataEdit::InputLoopFlag;
}

void ImageResourceDataEdit::InputLoopFlag(void)
{
	// loopFlag(��Ұ��݂�ٰ�ߍĐ������邩)�̓���(true or false)
	bool flg = false;
	inputHandle_[3] = KeyInputSingleCharString(100, 230, 100, inputLoopFlag_, true);
	// ���͂���������string�^�ɷ���
	auto loopStr = (std::string)inputLoopFlag_;
	// ���͂����l��"true"��������
	if (loopStr == "true")
	{
		// ��Ă����׸ނ�true��
		flg = true;
	}
	// ���͂����l�̾��
	lpSpriteDataCache.SetLoopFlag(flg);
	inputFunc_ = &ImageResourceDataEdit::InputDuration;
}

void ImageResourceDataEdit::InputDuration(void)
{
	// duration(1�ςɂ����鎞��)�̓���
	inputHandle_[4] = KeyInputSingleCharString(100, 330, 100, inputDuration_, true);
	std::string durationStr = inputDuration_;
	// ���͂����l�̾��(std::stof -> string�^��float�^�ɕύX)
	lpSpriteDataCache.SetDuration(std::stof(durationStr));
	// �����i�K�̈ڍs
	nowPhase_ = Phase::FOURTH;
}
