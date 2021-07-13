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
	// ���ع���ݏI���׸ނ̏�����
	exitAppFlag_ = false;
	// �e��ϐ��̏�����
	Initialize();
}

ColliderBoxEdit::~ColliderBoxEdit()
{
	// ���̼�݂��j�������ۂ�ؾ�Ă�����
	ResetData();
}

UniqueScene ColliderBoxEdit::Update(UniqueScene own, const std::shared_ptr<Mouse>& mouse)
{
	// �د����
	auto clickData = mouse->GetClick();
	// ϳ��߲�����߼޼��
	mPoint_ = mouse->GetPos();
	// ���E�د��ϐ������MAX�ɂ��Ă���
	// ���Ă������ƂŁA��ɉE�د�����Ă����Ԃ⍶�د�����Ă����Ԃ�����邽��
	clickDir_ = CLICK::MAX;
	// �د����ŉ�
	for (auto click : clickData)
	{
		// �د��ضް�L��
		if (click.second[static_cast<int>(TRG::NOW)] && !click.second[static_cast<int>(TRG::OLD)])
		{
			// ���د���
			if (click.first == INPUT_ID::START)
			{
				clickDir_ = CLICK::LEFT;
			}
			// �E�د���
			if (click.first == INPUT_ID::RESET)
			{
				clickDir_ = CLICK::RIGHT;
			}
		}
	}
	// ����̱�Ұ��ݖ����޼ު�Ė����ް�̧�ٌ��������ް�̧�ق�������Ȃ������Ƃ�
	if (!isFindFile_)
	{
		// ImageData�ҏW���݂�ϳ����ق�������
		if (HitCheck_FromCenter()(goImageDataButtonPos_, mPoint_, goImageDataButtonSize_))
		{
			// ϳ����ق���ɗ����׸ނ�true��
			isOnReturn_ = true;
			// ���د���
			if (clickDir_ == CLICK::LEFT)
			{
				// ImageDataEdit�Ɉڍs
				return std::make_unique<ImageResourceDataEdit>();
			}
		}
		else
		{
			// ϳ����ق����ĂȂ�
			isOnReturn_ = false;
		}
		// ϳ����ق����ع���ݏI�����݂̏�ɗ�����
		if (HitCheck_FromCenter()(exitAppButtonPos_, mPoint_, exitAppButtonSize_))
		{
			// ϳ����ق���ɗ����׸ނ�true��
			isOnExitApp_ = true;
			// ���د���
			if (clickDir_ == CLICK::LEFT)
			{
				// ���ع���ݏI���׸ނ�true�ɂ����ع���݂��I��
				exitAppFlag_ = true;
			}
		}
		else
		{
			// ϳ����ق����ĂȂ�
			isOnExitApp_ = false;
		}
	}
	
	ClsDrawScreen();
	// �`��
	Draw();
	ScreenFlip();
	// �X�V
	updater_[nowPhase_](mouse);
	return move(own);
}

void ColliderBoxEdit::Initialize(void)
{
	// FileManager�̐���(Singleton�ݽ�ݽ)
	FileManager::Create();
	// ����������݂��޼ު�Ė��̓��͂ɂ���
	inputFunc_ = &ColliderBoxEdit::InputObjectName;
	// �����ҏW��݂��Ұ��݂̺ς�I����������݂ɂ���
	editFunc_ = &ColliderBoxEdit::SelectFrame;
	// ���݂̒i�K��FIRST�ɂ���
	nowPhase_ = Phase::FIRST;
	// �e��ʻ��ނ̎擾
	auto vp = lpApplication.GetViewport();

	// �e���݂��߼޼�݂⻲�ނ̏�����--------------------------------------
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
	// ��Ұ��ݺς�\�����Ă��鏊��ϳ��߲�����������ɍX�V����鐔��
	// ���̺ϐ��Ɠ��������ɂȂ�
	onPointFrame_ = -1;
	// �ŏI�I�Ɍ��肵����Ұ��ݺς̏�����
	selectedFrame_ = onPointFrame_;
	// �ް�̧�ٌ����׸ނ̏�����
	isFindFile_ = false;
	// �`����`������(Attack(true) or Damage(false))
	typeFlag_ = false;
	// ImageDataEdit�ɖ߂����݂̏��
	// ϳ��߲���������׸ނ̏�����
	isOnReturn_ = false;
	// ���ع���ݏI�����݂̏��ϳ��߲���������׸ނ̏�����
	isOnExitApp_ = false;
	// �X�V�֐��B�̊i�[
	InitializeUpdater();
	// �`��֐��B�̊i�[
	InitializeDrawExcecuter();

	// �e�摜��۰�� ----------------------------------------------
	lpImageManager.Load("Images/saveButton.png");
	lpImageManager.Load("Images/goImageDataEditButton.png");
	lpImageManager.Load("Images/exit_application.png");
	// -----------------------------------------------------------
}

void ColliderBoxEdit::InitializeUpdater(void)
{
	// ���̼�݂�1�Ԗڂɍs����X�V����
	updater_.try_emplace(Phase::FIRST, [&](const std::shared_ptr<Mouse>& mouse) {
		// �e����͏���
		(this->*inputFunc_)();
		});
	// ���̼�݂�2�Ԗڂɍs����X�V����
	updater_.try_emplace(Phase::SECOND, [&](const std::shared_ptr<Mouse>& mouse) {
		// �ް�̧�ق̌���(���͵�޼ު�Ė��Ʊ�Ұ��ݖ���)
		isFindFile_ = lpFileManager.IsExistFile("Sprite",inputObjectName_, inputAnimationName_);
		// �ް�̧�ق����݂���
		if (isFindFile_)
		{
			// ���͉���Ұ��ݖ���string�ɷ���
			auto animName = (std::string)inputAnimationName_;
			// ���߽�ɐ���
			auto fullPath = "Images/Actor/" + (std::string)inputObjectName_ + "/" + animName;
			// ���͂�����޼ު�Ė��Ʊ�Ұ��ݖ�����̫��ޓ��ɑ��݂���̧�ق�����
			lpFileManager.SearchFileFromDirectory(fullPath);
			// �ް�̧�ق���ǂݎ�����摜�߽�ŉ摜��۰��
			for (auto& fileName : lpFileManager.GetFileNameList())
			{
				lpImageManager.Load("Images/Actor/" + (std::string)inputObjectName_ + 
					"/" + animName + "/" + fileName);
			}
			// �ڰѐ���\�����锠��resize
			frameNumBox_.resize(lpFileManager.GetFileNameList().size());
			// �����߼޼�݂Ƃ����ɏ����Ă��鐔���̐ݒ�
			for (int i = 0; i < frameNumBox_.size(); i++)
			{
				frameNumBox_[i].first = { 50 + (i * 50),30 };
				frameNumBox_[i].second = i;
			}
			// ���̏�����݂Ɉڍs
			nowPhase_ = Phase::THIRD;
		}
		});
	// ���̼�݂�3�Ԗڂɍs����X�V����
	updater_.try_emplace(Phase::THIRD, [&](const std::shared_ptr<Mouse>& mouse) {
		// �ҏW�̍ۂɍs����X�V����(����:���E�د�,ϳ��߲��)
		(this->*editFunc_)(clickDir_,mPoint_);
		});
}

void ColliderBoxEdit::InitializeDrawExcecuter(void)
{
	// ���̼�݂�1�Ԗڂɍs����X�V����
	drawExcecuter_.try_emplace(Phase::FIRST, [&]() {
		// ��޼ު�Ė��̓���̫��
		DrawFormatString(objectNameBoxPos_.x, objectNameBoxPos_.y - 25, 0xffffff, "��޼ު�Ė������");
		DrawBox(objectNameBoxPos_.x, objectNameBoxPos_.y,
			objectNameBoxPos_.x + objectNameBoxSize_.x, objectNameBoxPos_.y + objectNameBoxSize_.y,
			0xffffff, false);
		// ��Ұ��ݖ��̓���̫��
		DrawFormatString(animationNameBoxPos_.x, animationNameBoxPos_.y - 25, 0xffffff, "��Ұ��ݖ������");
		DrawBox(animationNameBoxPos_.x, animationNameBoxPos_.y,
			animationNameBoxPos_.x + animationNameBoxSize_.x, animationNameBoxPos_.y + animationNameBoxSize_.y,
			0xffffff, false);
		// ���͂�����޼ު�Ė��̕\��
		DrawFormatString(objectNameBoxPos_.x, objectNameBoxPos_.y, 0xffffff, "%s", inputObjectName_);
		// ���͂�����Ұ��ݖ��̕\��
		DrawFormatString(animationNameBoxPos_.x, animationNameBoxPos_.y, 0xffffff,"%s",inputAnimationName_);

		});
	// ���̼�݂�2�Ԗڂɍs����X�V����
	drawExcecuter_.try_emplace(Phase::SECOND, [&]() {
		if (!isFindFile_)
		{
			// �ް�̧�ٌ�������ۂ̕�����
			auto str = (std::string)inputObjectName_ + "_" + (std::string)inputAnimationName_;
			// �ް����Ȃ�����m�点��
			DrawFormatString(0, 0, 0xffffff,
				"�ް����L��܂���I%s%��ImageData�ҏW���ɏI�������ĉ������B",
				str.c_str());
			// ImageDataEdit�ɖ߂邽�߂����݂̕`��
			DrawRotaGraph(goImageDataButtonPos_.x, goImageDataButtonPos_.y,1.0f,0.0f,
				lpImageManager.GetHandle("Images/goImageDataEditButton.png")[0],
				true);
			// ���ع���ݏI���������݂̕`��
			DrawRotaGraph(exitAppButtonPos_.x, exitAppButtonPos_.y, 1.0f, 0.0f,
				lpImageManager.GetHandle("Images/exit_application.png")[0],
				true);

			// ����̏ꏊ��ϳ��߲�����������ɂ����ɔ��������ޯ����`��------------
			// ImageDataEdit�ɖ߂����ݏ�
			if (isOnReturn_)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				DrawBox(goImageDataButtonPos_.x - (goImageDataButtonSize_.x / 2),
					goImageDataButtonPos_.y - (goImageDataButtonSize_.y / 2),
					goImageDataButtonPos_.x + (goImageDataButtonSize_.x / 2),
					goImageDataButtonPos_.y + (goImageDataButtonSize_.y / 2), 0xff0000,true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			// ���ع���ݏI�������ݏ�
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
	// ���̼�݂�3�Ԗڂɍs����X�V����
	drawExcecuter_.try_emplace(Phase::THIRD, [&]() {
		// Application�׽���e��ʻ��ނ̎擾
		auto vp = lpApplication.GetViewport();
		// �w�i
		DrawBox(0, 0, vp.screen.x, vp.screen.y, 0x000fff, true);
		// ��׉摜�̽�ذݔw�i
		DrawBox(vp.editScreen.x - 100, vp.editScreen.y - 100,
			vp.screen.x, vp.screen.y, 0x000000, true);
		// �ڰт�\�������Ă����ɕ`��
		DrawFormatString(50,0, 0xffffff, "�e�ڰс�");
		// �e�ڰ����݂�1,2,3,4,5,6,7......�ƕ`�悵�Ă���
		for (int i = 0;i < frameNumBox_.size();i++)
		{
			DrawBox(frameNumBox_[i].first.x, frameNumBox_[i].first.y,
				frameNumBox_[i].first.x + frameNumBoxSize_.x,
				frameNumBox_[i].first.y + frameNumBoxSize_.y,
				0xffffff, false);
			DrawFormatString(frameNumBox_[i].first.x + 10,
				frameNumBox_[i].first.y + 10, 0xffffff, "%d", frameNumBox_[i].second);
			// saveFlag_��ػ��ނ���Ă���`��J�n
			if (saveFlag_.size() > 0)
			{
				// ���̺ς����ފ������Ă������ɐԐF�Łu�ρv�Ƃ���������\��
				if (saveFlag_[i])
				{
					DrawFormatString(frameNumBox_[i].first.x + 10, frameNumBox_[i].first.y - 15,
						0xff0000, "��");
				}
			}
		}
		// ϳ��߲���������ڰт����ݏ�ɗ�����
		// ϳ��߲���������ڰ����݂𔖂��΂œh��Ԃ�
		if (onPointFrame_ > -1)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			DrawBox(frameNumBox_[onPointFrame_].first.x, frameNumBox_[onPointFrame_].first.y,
				frameNumBox_[onPointFrame_].first.x + frameNumBoxSize_.x,
				frameNumBox_[onPointFrame_].first.y + frameNumBoxSize_.y, 0x00ff00, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		// �ҏW�Ώ۷�׉摜�̕`��ƕҏW�Ώۉ摜��̧�ٖ��̕`��
		if (selectedFrame_ != -1)
		{
			auto fileName = "Images/Actor/" + (std::string)inputObjectName_ +
				"/" + (std::string)inputAnimationName_ + "/" + lpFileManager.GetFileNameList()[selectedFrame_];
			DrawRotaGraph(objImagePos_.x, objImagePos_.y, 1.0f, 0.0f, lpImageManager.GetHandle(fileName)[0], true);
			DrawFormatString(vp.editScreen.x - 100, vp.editScreen.y - 125, 0xffffff, "%s",fileName.c_str());
		}
		// ��`�ҏW����݂���
		if (editFunc_ == &ColliderBoxEdit::BoxEdit)
		{
			// ϳ��߲���̐�ɋ�`���ޖ��̐F�ŏ����Ȋۂ�h��Ԃ��ŕ`��
			DrawCircle(mPoint_.x, mPoint_.y, 5, typeColor_, true);
		}
		// ��`���߂̕ύX���݂̕`��-------------------------------------------------------------
		DrawFormatString(typeChangerPos_[0].x - 80, typeChangerPos_[0].y, 0xffffff, "��`����:");
		for (auto typePos : typeChangerPos_)
		{
			DrawBox(typePos.x, typePos.y,
				typePos.x + typeChangerBoxSize_.x, typePos.y + typeChangerBoxSize_.y, 0xffffff, false);
		}
		DrawFormatString(typeChangerPos_[0].x + 5, typeChangerPos_[0].y + 5, 0xff0000, "�U����`");
		DrawFormatString(typeChangerPos_[1].x + 5, typeChangerPos_[1].y + 5, 0x00ff00, "��Ұ�ދ�`");
		// -------------------------------------------------------------------------------------
		// ��`���ߔԍ�
		auto typeNo = -1;
		// �U����`����
		if (typeFlag_)
		{
			// 0��
			typeNo = 0;
		}
		// ��Ұ�ދ�`����
		else
		{
			// 1��
			typeNo = 1;
		}
		// �U����`����Ұ�ދ�`�Ȃ��
		if (typeNo != -1)
		{
			// ���ݑI�����߂̉��Ɂ��`��
			// �U����`�@���@��@��Ұ�ދ�`�@�� �݂����ɕ`�悳����
			DrawFormatString(typeChangerPos_[typeNo].x + 120, typeChangerPos_[typeNo].y + 5,
				0xffffff, "��", true,10);
		}
		// boxManager_����������Ă��āA�ڰѐ����ݒ肳��Ă�����
		if (boxManager_ && selectedFrame_ != -1)
		{
			// ��`�̎n�_
			auto begPos = boxManager_->GetBegPos();
			// ��`�̎n�_���װ�߼޼�ݏ�ɂȂ��ꍇ
			if (begPos.x > -1 && begPos.y > -1)
			{
				// �摜�ҏW���(���F�̉��)��ϳ����L���
				if (HitCheck_FromLeftUp()
					(Vector2I(vp.editScreen.x - 100, vp.editScreen.y - 100), mPoint_,
					 Vector2I((vp.screen.x - (vp.editScreen.x - 100)),
							 ((vp.screen.y - (vp.editScreen.y - 100))))))
				{
					// ���ݕҏW���Ă����`�̕`��
					DrawBox(begPos.x, begPos.y, mPoint_.x, mPoint_.y, typeColor_, false);
				}
			}
			// boxManager_���ɒǉ����ꂽcolliderBox�B�̕`��(�w�肵���ڰт̋�`��)
			boxManager_->Draw(selectedFrame_);
		}
		// �������݂̕`��
		DrawGraph(saveButtonPos_.x, saveButtonPos_.y,
			lpImageManager.GetHandle("Images/saveButton.png")[0], true);
		// �������݂̎���ɔ���`��
		DrawBox(saveButtonPos_.x, saveButtonPos_.y,
			saveButtonPos_.x + saveButtonSize_.x, saveButtonPos_.y + saveButtonSize_.y,
			0xffffff, false);
		});

}

void ColliderBoxEdit::Draw(void)
{
	// �e�i�K���̕`��
	drawExcecuter_[nowPhase_]();
}

void ColliderBoxEdit::InputObjectName(void)
{
	// ��޼ު�Ė��̓���
	if (KeyInputSingleCharString(objectNameBoxPos_.x, objectNameBoxPos_.y, 20, inputObjectName_, true) == 1)
	{
		// �I�����ɱ�Ұ��ݖ����͂Ɉڍs
		inputFunc_ = &ColliderBoxEdit::InputAnimationName;
	}
}

void ColliderBoxEdit::InputAnimationName(void)
{
	// ��Ұ��ݖ��̓���
	if (KeyInputSingleCharString(animationNameBoxPos_.x, animationNameBoxPos_.y, 20, inputAnimationName_, true) == 1)
	{
		// ���̏�����ۯ��Ɉڍs
		nowPhase_ = Phase::SECOND;
	}
}

void ColliderBoxEdit::SelectFrame(const CLICK& click, const Vector2I& mPoint)
{
	// ϳ��߲�����ǂ��ڰѐ������݂ɗ��Ă��邩�̕ϐ���
	// �װ�l���i�[������(���)
	onPointFrame_ = -1;
	// �ڰт̐������ŉ�
	for (auto frameBox : frameNumBox_)
	{
		// ϳ��߲����������ڰєԍ��̏�ɗ�����
		if (HitCheck_FromLeftUp()(frameBox.first, mPoint, frameNumBoxSize_))
		{
			// ϳ��߲������ɗ����ڰт��i�[
			onPointFrame_ = frameBox.second;
			// ���د���
			if (clickDir_ == CLICK::LEFT)
			{
				// �I���ڰт�ϳ��߲�������Ă����ڰтɂ���
				selectedFrame_ = onPointFrame_;
				if (!boxManager_)
				{
					// �ޯ���Ȱ�ެ��̐���
					boxManager_ = new ColliderBoxManager(frameNumBox_.size());
				}

				saveFlag_.resize(frameNumBox_.size());
				// ���̕ҏW�i�K�Ɉڍs
				editFunc_ = &ColliderBoxEdit::BoxEdit;
			}
		}
	}
}

void ColliderBoxEdit::BoxEdit(const CLICK& click, const Vector2I& mPoint)
{
	// ��`���߂̕ύX------------------------------------------------------------
	// 0:�U����`���߁@1:��Ұ�ދ�`����
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
	// �U����`�ҏW����
	if (typeFlag_)
	{
		// �Ԃ�
		typeColor_ = 0xff0000;
	}
	// ��Ұ�ދ�`�ҏW����
	else
	{
		// ��
		typeColor_ = 0x00ff00;
	}
	// �e��ʻ��ނ̎擾
	auto vp = lpApplication.GetViewport();
	// �ҏW��ʏ��ϳ��߲�������݂����
	if (HitCheck_FromLeftUp()
		(Vector2I(vp.editScreen.x - 100, vp.editScreen.y - 100), mPoint_,
			Vector2I((vp.screen.x - (vp.editScreen.x - 100)),
				((vp.screen.y - (vp.editScreen.y - 100))))))
	{
		// ���د���
		if (click == CLICK::LEFT)
		{
			// ��`�̎n�_�̐ݒ�
			boxManager_->SetBegPos(mPoint);
		}
		// �E�د���
		if (click == CLICK::RIGHT)
		{
			// �I�_�����܂��������Ӗ�����̂�
			// ���̎��_��colliderBox��Add
			boxManager_->AddColliderBox(selectedFrame_, mPoint, typeFlag_);
		}
	}
	// �������ݏ��ϳ��߲�������݂����
	if (HitCheck_FromLeftUp()(saveButtonPos_, mPoint_, saveButtonSize_))
	{
		// ���د���
		if (click == CLICK::LEFT)
		{
			// ���݂̋�`�ް����ނ����׸ނ�true��
			saveFlag_[selectedFrame_] = true;
			// ��Ұ��ݺϐ��I���ɕύX
			editFunc_ = &ColliderBoxEdit::SelectFrame;
			// ϳ��߲�������݂����ڰѐ��̏�����
			onPointFrame_ = -1;
			// ���莞���ڰѐ��̏�����
			selectedFrame_ = onPointFrame_;
			// �S�Ă��ڰщ摜�̕ҏW�̏I���׸ޑS�Ăŉ�
			auto flg = true;
			for (auto frameFlg : saveFlag_)
			{
				// 1�ł�false����false�ɂȂ�悤�ɘ_���ς����
				flg &= frameFlg;
			}
			// �S�Ă��ڰщ摜���ҏW�I�����Ă�����
			if (flg)
			{
				// ���ނ����ް�̧�ٖ�
				std::string fileName = (std::string)inputObjectName_ + "/Rect/" + (std::string)inputAnimationName_;
				// �ް�̧�ق��쐬���A���̒����ް�����������ł���
				lpFileManager.WriteRectDataFile(fileName, *boxManager_);
				// �e�ϐ�����ؾ��
				ResetData();
			}
		}
	}
}

void ColliderBoxEdit::ResetData(void)
{
	// ����������݂��޼ު�Ė��̓��͂ɂ���
	inputFunc_ = &ColliderBoxEdit::InputObjectName;
	// �����ҏW��݂��ڰёI������݂ɂ���
	editFunc_ = &ColliderBoxEdit::SelectFrame;
	// �e���͖��̏�����
	for (int i = 0; i < 20; i++)
	{
		inputObjectName_[i] = ' ';
		inputAnimationName_[i] = ' ';
	}
	// boxManager��delete(�J��)
	delete boxManager_;
	// boxManager_����ɂ��Ă���
	boxManager_ = nullptr;
	onPointFrame_ = -1;
	selectedFrame_ = onPointFrame_;
	// �ڰ����݂��߼޼�ݺ��ł���ɂ���
	frameNumBox_.clear();
	// ���ފ����׸ނ̒��g����ɂ���
	saveFlag_.clear();
	// �ް�̧�ٌ����׸ނ̏�����
	isFindFile_ = false;
	// ��`���ߕϐ��̏�����
	typeFlag_ = false;
	// �����i�K�������ɖ߂�
	nowPhase_ = Phase::FIRST;
}
